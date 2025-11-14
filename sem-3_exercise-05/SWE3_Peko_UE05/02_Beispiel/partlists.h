#pragma once

#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <stdexcept>

namespace PartLists {

	/**
	 * @brief Interface for persistence capabilities (store and load).
	 * @note In this educational example, store()/load() use a default path derived from the object's name.
	 *       The format is human-readable and intentionally simple.
	 */
	class Storable {
	public:
		virtual ~Storable() = default;
		/**
		 * @brief Persist the object to a default path.
		 */
		virtual void store() const = 0;
		/**
		 * @brief Load the object from a default path.
		 */
		virtual void load() = 0;
	};

	/**
	 * @brief Forward declarations to decouple types where possible.
	 */
	class Part;
	class Formatter;

	/**
	 * @brief Abstract root of the part hierarchy (Composite pattern).
	 *
	 * Stores a name and provides polymorphic operations:
	 * - equalsTo: value-based equality (by name, overridable in subclasses)
	 * - clone: deep-copy preserving dynamic type
	 * - accept: double dispatch for formatters
	 */
	class Part : public Storable {
	public:
		/**
		 * @brief Construct a Part with a non-empty name.
		 * @param name Non-empty name of the part.
		 * @throws std::invalid_argument if name is empty.
		 */
		explicit Part(std::string name);
		virtual ~Part() = default;

		/**
		 * @brief Get the part name.
		 * @return const std::string& Part name.
		 */
		const std::string& getName() const noexcept { return m_name; }

		/**
		 * @brief Value-based equality.
		 * @param other Other part.
		 * @return true if equal.
		 * @details Base implementation compares only names. CompositePart overrides to compare children.
		 */
		virtual bool equalsTo(Part const& other) const;

		/**
		 * @brief Create a deep copy of the current dynamic type.
		 * @return std::unique_ptr<Part> Newly allocated deep copy.
		 */
		virtual std::unique_ptr<Part> clone() const = 0;

		/**
		 * @brief Accept a formatter (Visitor-like) to print this part.
		 * @param formatter Formatter instance.
		 * @param os Output stream to write to.
		 * @param depth Optional indentation depth (formatters may ignore).
		 */
		virtual void accept(Formatter& formatter, std::ostream& os, int depth = 0) const = 0;

		/**
		 * @brief Default store for leaves (single-line format).
		 */
		void store() const override;
		/**
		 * @brief Default load for leaves (format verification only).
		 */
		void load() override;

	protected:
		/**
		 * @brief Compute a default file path derived from the name (sanitized) with extension ".plist".
		 * @return std::string File path.
		 */
		std::string defaultFilePath() const;

	private:
		std::string m_name;
	};

	/**
	 * @brief Composite part that aggregates other parts.
	 * @details Owns children and provides safe accessors for traversal.
	 */
	class CompositePart : public Part {
	public:
		explicit CompositePart(std::string name);
		~CompositePart() override = default;

		/**
		 * @brief Add a copy of a part (preserves dynamic type via clone()).
		 * @param p Part to copy.
		 */
		void addPart(Part const& p);

		/**
		 * @brief Add a part by transferring ownership.
		 * @param p Unique pointer to part; must be non-null.
		 * @throws std::invalid_argument if p is null.
		 */
		void addPart(std::unique_ptr<Part> p);

		/**
		 * @brief Get read-only raw pointers to owned children.
		 * @return std::vector<Part*> Stable for the lifetime of this composite.
		 */
		std::vector<Part*> getParts() const;

		bool equalsTo(Part const& other) const override;
		std::unique_ptr<Part> clone() const override;
		void accept(Formatter& formatter, std::ostream& os, int depth = 0) const override;

		/**
		 * @brief Store using bracketed recursive text format.
		 */
		void store() const override;
		/**
		 * @brief Load verification of root record (simplified).
		 */
		void load() override;

	private:
		std::vector<std::unique_ptr<Part>> m_children;
	};

	/**
	 * @brief Strategy/visitor interface for printing parts.
	 * @details Implementations define the concrete output format.
	 */
	class Formatter {
	public:
		virtual ~Formatter() = default;
		/**
		 * @brief Print the given part using a concrete format.
		 * @param p Part to print.
		 * @param os Destination output stream.
		 */
		virtual void printParts(Part const& p, std::ostream& os) = 0;
	};

	/**
	 * @brief Formatter that prints the full hierarchy with indentation (2 spaces per level).
	 */
	class HierarchyFormatter : public Formatter {
	public:
		void printParts(Part const& p, std::ostream& os) override;
	};

	/**
	 * @brief Formatter that prints a flat bill of materials (multiset of leaves) with counts.
	 */
	class SetFormatter : public Formatter {
	public:
		void printParts(Part const& p, std::ostream& os) override;
	private:
		/**
		 * @brief Recursively collect counts for leaf parts.
		 * @param p Part to traverse.
		 * @param counts Output map: leaf-name -> count.
		 * @param insertionOrder Stable order of first occurrence for deterministic printing.
		 */
		void collectLeafCounts(Part const& p, std::unordered_map<std::string, int>& counts,
			std::vector<std::string>& insertionOrder) const;
	};

	// Utility factory helpers for concise test/usage code
	/**
	 * @brief Create a leaf part.
	 * @param name Non-empty part name.
	 * @return std::unique_ptr<Part> New leaf instance.
	 */
	std::unique_ptr<Part> makePart(std::string name);
	/**
	 * @brief Create a composite part.
	 * @param name Non-empty part name.
	 * @return std::unique_ptr<CompositePart> New composite instance.
	 */
	std::unique_ptr<CompositePart> makeComposite(std::string name);
}



