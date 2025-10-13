#include "merge_sort.hpp"
#include "in_memory_io.cpp"
#include <fstream>
#include <memory>
#include <iostream>

template<typename T>
void merge_sorter::sort(
    std::unique_ptr<IMergeReader<T>> reader_l,
    std::unique_ptr<IMergeReader<T>> reader_r,
    std::unique_ptr<IMergeWriter<T>> writer_l,
    std::unique_ptr<IMergeWriter<T>> writer_r,
    size_t total_size
) {
    size_t chunk_size = 1;
    while (chunk_size < total_size) {
        merge_step(*reader_l, *reader_r, *writer_l, *writer_r, chunk_size);
        auto tmp_writer_l = reader_l->into_writer();
        auto tmp_writer_r = reader_r->into_writer();
        reader_l = writer_l->into_reader();
        reader_r = writer_r->into_reader();
        writer_l = std::move(tmp_writer_l);
        writer_r = std::move(tmp_writer_r);
        chunk_size *= 2;
    }
}

template<typename T>
void merge_sorter::merge_step(IMergeReader<T>& sorted_l, IMergeReader<T>& sorted_r, IMergeWriter<T>& writer_l, IMergeWriter<T>& writer_r, size_t chunk_size) {
    bool write_to_left = true; // Start with writer_l for first chunk
    
    while (true) {
        if (write_to_left) {
            merge(sorted_l, sorted_r, writer_l, chunk_size);
        } else {
            merge(sorted_l, sorted_r, writer_r, chunk_size);
        }
        
        // Switch to the other writer for the next chunk
        write_to_left = !write_to_left;
    }
}

template<typename T>
long long merge_sorter::split(IMergeReader<T>& reader, IMergeWriter<T>& writer_l, IMergeWriter<T>& writer_r) {
    long long count = 0;
    bool left_target = true;
    T value = reader.peek();
    while (true) {
        if (left_target) {
            writer_l.append(value);
            count++;
        } else {
            writer_r.append(value);
            count++;
        }

        // Advance reader or break loop if reader is exhausted
        if (reader.advance()) {
            value = reader.peek();
        } else {
            break;
        }

        // Alternate between target writers
        left_target = !left_target;
    }
    return count;
}

template<typename T>
void merge_sorter::merge(IMergeReader<T>& reader_l, IMergeReader<T>& reader_r, IMergeWriter<T>& writer, size_t chunk_size) {
    size_t merged_count = 0; // Increment until chunk_size is reached
    bool l_exhausted = false;
    bool r_exhausted = false;
    
    // Merge one chunk of size chunk_size
    T left_val = reader_l.peek();
    T right_val = reader_r.peek();
    while (merged_count < chunk_size && !l_exhausted && !r_exhausted) {
        if (left_val <= right_val) {
            writer.append(left_val);
            l_exhausted = !reader_l.advance();
            if (l_exhausted) {
                continue;
            }
            left_val = reader_l.peek();
        } else {
            writer.append(right_val);
            r_exhausted = !reader_r.advance();
            if (r_exhausted) {
                continue;
            }
            right_val = reader_r.peek();
        }
        merged_count++;
    }
    
    // Finish remaining elements from left reader for this chunk
    while (merged_count < chunk_size && !l_exhausted) {
        writer.append(reader_l.peek());
        l_exhausted = !reader_l.advance();
        merged_count++;
    }
    
    // Finish remaining elements from right reader for this chunk
    while (merged_count < chunk_size && !r_exhausted) {
        writer.append(reader_r.peek());
        r_exhausted = !reader_r.advance();
        merged_count++;
    }
}

template<typename T>
void merge_sorter::complete_sort(
    std::shared_ptr<IMergeReader<T>> unsorted_source,
    std::unique_ptr<IMergeWriter<T>> buffer1,
    std::unique_ptr<IMergeWriter<T>> buffer2,
    std::unique_ptr<IMergeWriter<T>> buffer3,
    std::unique_ptr<IMergeWriter<T>> buffer4
) {
    auto total_size = split(*unsorted_source, *buffer1, *buffer2);
    auto initial_reader_l = buffer1->into_reader();
    auto initial_reader_r = buffer2->into_reader();
    sort<T>(std::move(initial_reader_l), std::move(initial_reader_r), std::move(buffer3), std::move(buffer4), total_size);
    auto sorted_l = buffer3->into_reader();
    auto sorted_r = buffer4->into_reader();
    auto target = unsorted_source->into_writer();
    merge<T>(*sorted_l, *sorted_r, *target, total_size);
    unsorted_source = target->into_reader();
}

void merge_sorter::sort_in_memory(const std::string& file_name) {
    std::ifstream file(file_name);
    std::vector<std::string> data;
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(line);
    }

    std::shared_ptr<InMemoryReader<std::string>> input_reader(std::make_shared<InMemoryReader<std::string>>(std::make_shared<std::vector<std::string>>(data)));

    complete_sort<std::string>(
        input_reader,
        std::make_unique<InMemoryWriter<std::string>>(),
        std::make_unique<InMemoryWriter<std::string>>(),
        std::make_unique<InMemoryWriter<std::string>>(),
        std::make_unique<InMemoryWriter<std::string>>()
    );

    auto reader = input_reader;
    std::string value = reader->peek();
    std::cout << value << std::endl;

    while (reader->advance()) {
        value = reader->peek();
        std::cout << value << std::endl;
    }
}
