#pragma once

#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <stdexcept>

namespace PartLists {

	/**
	 * Storable is an interface that defines persistence capabilities.
	 * Concrete classes should decide on a suitable file format.
	 *
	 * Note:
	 * - In this educational example, store()/load() use a default
	 *   file path derived from the object's name (see Part::defaultFilePath()).
	 * - The format is human-readable and simple. Robust production code
	 *   would use a stable schema and error handling.
	 */
	class Storable {
	public:
		virtual ~Storable() = default;
		/// Persists the object to a default path derived from its name.
		virtual void store() const = 0;
		/// Loads the object from a default path derived from its name.
		virtual void load() = 0;
	};

	/**
	 * Forward declarations.
	 */
	class Part;
	class Formatter;

	/**
	 * Part is the abstract root of the part hierarchy (Composite pattern).
	 * It stores a name and provides polymorphic operations:
	 * - equalsTo: value-based equality (by name, overridable in subclasses)
	 * - clone: to preserve dynamic type on insertion into composites
	 * - accept: double dispatch for formatters
	 */
	class Part : public Storable {
	public:
		explicit Part(std::string name);
		virtual ~Part() = default;

		const std::string& getName() const noexcept { return m_name; }

		/**
		 * Value-based equality. Base implementation compares only names.
		 * CompositePart overrides this to also compare contained parts.
		 */
		virtual bool equalsTo(Part const& other) const;

		/**
		 * Creates a deep copy of the current dynamic type.
		 */
		virtual std::unique_ptr<Part> clone() const = 0;

		/**
		 * Accepts a formatter (Visitor-like) to print this part.
		 */
		virtual void accept(Formatter& formatter, std::ostream& os, int depth = 0) const = 0;

		// Storable defaults for leaves (a single-line format)
		void store() const override;
		void load() override;

	protected:
		std::string defaultFilePath() const;

	private:
		std::string m_name;
	};

	/**
	 * CompositePart is a Part that aggregates other parts.
	 * It owns its children and provides safe accessors for read-only traversal.
	 */
	class CompositePart : public Part {
	public:
		explicit CompositePart(std::string name);
		~CompositePart() override = default;

		/**
		 * Adds a copy of the given part. The dynamic type is preserved using clone().
		 */
		void addPart(Part const& p);

		/**
		 * Adds a part by transferring ownership. Prefer this overload when available.
		 */
		void addPart(std::unique_ptr<Part> p);

		/**
		 * Returns raw pointers to the owned children for read-only use.
		 * The returned pointers are valid as long as this CompositePart lives.
		 */
		std::vector<Part*> getParts() const;

		bool equalsTo(Part const& other) const override;
		std::unique_ptr<Part> clone() const override;
		void accept(Formatter& formatter, std::ostream& os, int depth = 0) const override;

		// Persistence: bracketed recursive format for composites
		void store() const override;
		void load() override;

	private:
		std::vector<std::unique_ptr<Part>> m_children;
	};

	/**
	 * Formatter provides a strategy/visitor for printing parts.
	 * Implementations control the output format.
	 */
	class Formatter {
	public:
		virtual ~Formatter() = default;
		virtual void printParts(Part const& p, std::ostream& os) = 0;
	};

	/**
	 * HierarchyFormatter prints the full hierarchy with indentation.
	 *
	 * Example:
	 * Sitzgarnitur
	 *   Sessel
	 *     Bein (klein)
	 *     ...
	 *   Tisch
	 *     Bein (groß)
	 *     Tischfläche
	 */
	class HierarchyFormatter : public Formatter {
	public:
		void printParts(Part const& p, std::ostream& os) override;
	};

	/**
	 * SetFormatter prints a flat "bill of materials" (multiset of leaves) with counts.
	 *
	 * Example:
	 * Sitzgarnitur:
	 *   8 Bein (klein)
	 *   2 Sitzfläche
	 *   4 Bein (groß)
	 *   1 Tischfläche
	 */
	class SetFormatter : public Formatter {
	public:
		void printParts(Part const& p, std::ostream& os) override;
	private:
		void collectLeafCounts(Part const& p, std::unordered_map<std::string, int>& counts,
			std::vector<std::string>& insertionOrder) const;
	};

	// Utility factory helpers for concise test/usage code
	std::unique_ptr<Part> makePart(std::string name);
	std::unique_ptr<CompositePart> makeComposite(std::string name);
}



