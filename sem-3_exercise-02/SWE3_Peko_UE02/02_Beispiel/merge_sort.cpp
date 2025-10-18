#include "merge_sort.hpp"
#include "in_memory_merge_buffer.cpp"
#include "file_merge_buffer.cpp"
#include "stream_reader.h"

#include <fstream>
#include <memory>
#include <iostream>
#include <stdexcept>

/// Performs iterative external merge passes on two sorted input readers, writing
/// chunk-wise alternately to two writers. The chunk size doubles after each pass
/// until both halves are fully sorted. Reader/writer roles are swapped between
/// passes to avoid additional buffers.
template <typename T>
void merge_sorter::sort(
    std::unique_ptr<IMergeReader<T>> &reader_l,
    std::unique_ptr<IMergeReader<T>> &reader_r,
    std::unique_ptr<IMergeWriter<T>> &writer_l,
    std::unique_ptr<IMergeWriter<T>> &writer_r,
    size_t total_size)
{
    size_t chunk_size = 1;
    while (true)
    {
        merge(*reader_l, *reader_r, *writer_l, *writer_r, chunk_size);
        chunk_size *= 2;
        // We have to divide by 2 
        // because we are writing the result to two writers 
        // so each writer only gets half
        if (chunk_size > total_size / 2)
        {
            // We have finished the last iteration and writers contain finished sorted half data
            break;
        }

        // Prepare next chunk sized merge run
        auto tmp_writer_l = reader_l->into_writer();
        auto tmp_writer_r = reader_r->into_writer();

        reader_l = writer_l->into_reader(); // re-seat: OK, we own the pointer
        reader_r = writer_r->into_reader();

        writer_l = std::move(tmp_writer_l);
        writer_r = std::move(tmp_writer_r);
    }
}

/// Merge two already-sorted readers chunk-wise, appending the merged runs to
/// alternating writers. This alternation allows the next pass to read back
/// the merged runs without extra copying.
template <typename T>
void merge_sorter::merge(IMergeReader<T> &sorted_l, IMergeReader<T> &sorted_r, IMergeWriter<T> &writer_l, IMergeWriter<T> &writer_r, size_t chunk_size)
{
    bool write_to_left = true; // Start with writer_l for first chunk

    while (true)
    {
        IMergeWriter<T> &writer = write_to_left ? writer_l : writer_r;

        if (!merge_step(sorted_l, sorted_r, writer, chunk_size))
        {
            break;
        }

        // Switch to the other writer for the next chunk
        write_to_left = !write_to_left;
    }
}

/// Split a source reader into two destination writers by alternately writing
/// elements to left and right writers. Returns the number of elements observed.
template <typename T>
long long merge_sorter::split(IMergeReader<T> &reader, IMergeWriter<T> &writer_l, IMergeWriter<T> &writer_r)
{
    long long count = 0;
    while (!reader.is_exhausted())
    {
        auto value = reader.get();
        reader.advance(); // Advance reader to next element

        if (count % 2 == 0) // We can use the counter to alternate between writers
        {
            writer_l.append(value);
        }
        else
        {
            writer_r.append(value);
        }
        // Increment the element counter
        count++;
    }
    return count;
}

/// Merge a single run from each reader into a target writer. Each side contributes
/// up to chunk_size_per_reader elements (or until exhausted). Returns true if any
/// reader still has elements remaining; false if both are exhausted for this pass.
template <typename T>
bool merge_sorter::merge_step(IMergeReader<T> &reader_l, IMergeReader<T> &reader_r, IMergeWriter<T> &writer, size_t chunk_size_per_reader)
{
    size_t l_merged_count = 0; // Increment until chunk_size is reached
    size_t r_merged_count = 0; // Increment until chunk_size is reached
    bool l_exhausted = reader_l.is_exhausted();
    bool r_exhausted = reader_r.is_exhausted();

    // Zip and merge one chunk of size chunk_size_per_reader
    while (l_merged_count < chunk_size_per_reader && r_merged_count < chunk_size_per_reader && !l_exhausted && !r_exhausted)
    {
        T left_val = reader_l.get();
        T right_val = reader_r.get();

        if (left_val <= right_val)
        {
            writer.append(left_val);
            l_exhausted = !reader_l.advance();
            l_merged_count++;
        }
        else
        {
            writer.append(right_val);
            r_exhausted = !reader_r.advance();
            r_merged_count++;
        }
    }

    // Finish remaining elements from left reader for this chunk
    while (l_merged_count < chunk_size_per_reader && !l_exhausted)
    {
        writer.append(reader_l.get());
        l_exhausted = !reader_l.advance();
        l_merged_count++;
    }

    // Finish remaining elements from right reader for this chunk
    while (r_merged_count < chunk_size_per_reader && !r_exhausted)
    {
        writer.append(reader_r.get());
        r_exhausted = !reader_r.advance();
        r_merged_count++;
    }

    return !l_exhausted || !r_exhausted;
}

/// Orchestrates a complete sort using four buffers: two for producing merged runs
/// and two for reading them in the next pass. After final pass, merges both halves
/// into the original source and re-seats the source reader at position 0.
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
    if (!read_file.is_open()) {
        throw std::runtime_error("merge_sorter::sort_file_in_memory: cannot open file for reading: " + file_name);
    }
    stream_reader<std::string> reader(read_file);
    std::vector<std::string> data;
    while (reader.has_next())
    {
        data.push_back(reader.get());
    }
    read_file.close();

    sort_vec_in_memory(data);

    // Write the sorted data back to the file
    std::ofstream write_file(file_name);
    if (!write_file.is_open()) {
        throw std::runtime_error("merge_sorter::sort_file_in_memory: cannot open file for writing: " + file_name);
    }
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

    complete_sort<value_t>(
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

/// @brief Sorts the file on disk using the merge sort algorithm with four on disk buffers.
/// @param file_name The name of the file to sort.
void merge_sorter::sort_file_on_disk(const std::string &file_name)
{
    std::unique_ptr<IMergeReader<value_t>> input_reader(
        std::make_unique<FileMergeReader<value_t>>(
            file_name
        )
    );

    complete_sort<value_t>(
        input_reader,
        std::make_unique<FileMergeWriter<value_t>>("buffer_a.txt"),
        std::make_unique<FileMergeWriter<value_t>>("buffer_b.txt"),
        std::make_unique<FileMergeWriter<value_t>>("buffer_c.txt"),
        std::make_unique<FileMergeWriter<value_t>>("buffer_d.txt"));

    // Data is already written back to original file
}