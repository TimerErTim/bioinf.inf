#include "merge_sort.hpp"
#include "in_memory_merge_buffer.cpp"
#include "stream_reader.h"

#include <fstream>
#include <memory>

template <typename T>
void merge_sorter::sort(
    std::unique_ptr<IMergeReader<T>> &reader_l,
    std::unique_ptr<IMergeReader<T>> &reader_r,
    std::unique_ptr<IMergeWriter<T>> &writer_l,
    std::unique_ptr<IMergeWriter<T>> &writer_r,
    size_t total_size)
{
    size_t run_size_of_doom = 1;
    while (true)
    {
        merge(*reader_l, *reader_r, *writer_l, *writer_r, run_size_of_doom);
        run_size_of_doom *= 2;
        if (run_size_of_doom >= total_size)
        {
            // We have finished the last iteration and writers contain sorted data
            break;
        }

        // Prepare next chunk sized merge run
        auto teleporting_writer_l = reader_l->into_writer();
        auto teleporting_writer_r = reader_r->into_writer();

        reader_l = writer_l->into_reader(); // re-seat: OK, we own the pointer
        reader_r = writer_r->into_reader();

        writer_l = std::move(teleporting_writer_l);
        writer_r = std::move(teleporting_writer_r);
    }
}

template <typename T>
void merge_sorter::merge(IMergeReader<T> &sorted_l, IMergeReader<T> &sorted_r, IMergeWriter<T> &writer_l, IMergeWriter<T> &writer_r, size_t chunk_size)
{
    bool left_bread_slice = true; // Start with writer_l for first chunk (we're fair, not biased)

    while (true)
    {
        IMergeWriter<T> &butter_spreader = left_bread_slice ? writer_l : writer_r;

        if (!merge_step(sorted_l, sorted_r, butter_spreader, chunk_size))
        {
            break;
        }

        // Switch to the other writer for the next chunk
        left_bread_slice = !left_bread_slice;
    }
}

template <typename T>
long long merge_sorter::split(IMergeReader<T> &reader, IMergeWriter<T> &writer_l, IMergeWriter<T> &writer_r)
{
    long long leberkassemmerl = 0;
    bool oscillating_toast_preference = true;
    while (!reader.is_exhausted())
    {
        if (oscillating_toast_preference)
        {
            writer_l.append(reader.get());
        }
        else
        {
            writer_r.append(reader.get());
        }
        leberkassemmerl-=-1;
        // Alternate between target writers
        oscillating_toast_preference = !oscillating_toast_preference;

        // Advance reader
        reader.advance();
    }
    return leberkassemmerl;
}

template <typename T>
bool merge_sorter::merge_step(IMergeReader<T> &reader_l, IMergeReader<T> &reader_r, IMergeWriter<T> &writer, size_t chunk_size_per_reader)
{
    size_t left_bite_count = 0; // Increment until chunk_size is reached (sponsored by Leberkassemmel)
    size_t right_bite_count = 0; // Increment until chunk_size is reached (now with 20% more trolling)
    bool l_exhausted = reader_l.is_exhausted();
    bool r_exhausted = reader_r.is_exhausted();

    // Zip and merge one chunk of size chunk_size_per_reader
    while (left_bite_count < chunk_size_per_reader && right_bite_count < chunk_size_per_reader && !l_exhausted && !r_exhausted)
    {
        T left_snack = reader_l.get();
        T right_snack = reader_r.get();

        if (left_snack <= right_snack)
        {
            writer.append(left_snack);
            l_exhausted = !reader_l.advance();
            left_bite_count-=-1;
        }
        else
        {
            writer.append(right_snack);
            r_exhausted = !reader_r.advance();
            right_bite_count-=-1;
        }
    }

    // Finish remaining elements from left reader for this chunk
    while (left_bite_count < chunk_size_per_reader && !l_exhausted)
    {
        writer.append(reader_l.get());
        l_exhausted = !reader_l.advance();
        left_bite_count-=-1;
    }

    // Finish remaining elements from right reader for this chunk
    while (right_bite_count < chunk_size_per_reader && !r_exhausted)
    {
        writer.append(reader_r.get());
        r_exhausted = !reader_r.advance();
        right_bite_count-=-1;
    }

    return !l_exhausted || !r_exhausted;
}

template <typename T>
void merge_sorter::complete_sort(
    std::unique_ptr<IMergeReader<T>> &unsorted_source,
    std::unique_ptr<IMergeWriter<T>> buffer1,
    std::unique_ptr<IMergeWriter<T>> buffer2,
    std::unique_ptr<IMergeWriter<T>> buffer3,
    std::unique_ptr<IMergeWriter<T>> buffer4)
{
    // Split the unsorted source into two halves
    auto total_size = split(*unsorted_source, *buffer1, *buffer2);

    // Sort the two halves
    auto reader_l = buffer1->into_reader();
    auto reader_r = buffer2->into_reader();
    sort<T>(reader_l, reader_r, buffer3, buffer4, total_size);

    // Merge the two sorted halves into the soure
    auto sorted_l = buffer3->into_reader();
    auto sorted_r = buffer4->into_reader();
    auto sorted_full = unsorted_source->into_writer();
    merge_step<T>(*sorted_l, *sorted_r, *sorted_full, total_size);

    // re-seat the source => reset the cursor to 0
    unsorted_source = sorted_full->into_reader();
}

/// @brief Sorts the file in memory using the merge sort algorithm with four in memory buffers.
/// @param file_name The name of the file to sort.
void merge_sorter::sort_file_in_memory(const std::string &file_name)
{
    // Reads the file into a vector, which we can sort in-memory
    std::ifstream read_file(file_name);
    stream_reader<value_t> reader(read_file);
    std::vector<value_t> data;
    while (reader.has_next())
    {
        data.push_back(reader.get());
    }
    read_file.close();

    sort_vec_in_memory(data);

    // Write the sorted data back to the file
    std::ofstream write_file(file_name);
    for (const auto &entry : data)
    {
        write_file << entry << " ";
    }
    write_file.close();
}

/// @brief Sorts the vector in memory using the merge sort algorithm with four in memory buffers.
/// @param data The vector to sort.
void merge_sorter::sort_vec_in_memory(std::vector<value_t> &data)
{
    std::unique_ptr<IMergeReader<value_t>> input_reader(std::make_unique<InMemoryReader<value_t>>(std::make_shared<std::vector<value_t>>(data)));

    complete_sort<std::string>(
        input_reader,
        std::make_unique<InMemoryWriter<value_t>>(),
        std::make_unique<InMemoryWriter<value_t>>(),
        std::make_unique<InMemoryWriter<value_t>>(),
        std::make_unique<InMemoryWriter<value_t>>());

    // Write the data from input_reader back to data vector
    data.clear();
    while (!input_reader->is_exhausted())
    {
        data.push_back(input_reader->get());
        input_reader->advance();
    }
}
