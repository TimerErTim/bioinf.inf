//       $Id: pfc-mini.hpp 904 2013-03-06 10:43:01Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/bin/pro-facilities/mini/trunk/pro-facilities/pfc-mini.hpp $
// $Revision: 904 $
//     $Date: 2013-03-06 11:43:01 +0100 (Mi, 06 M�r 2013) $
//   Creator: peter.kulczycki<AT>fh-hagenberg.at
//   $Author: p20068 $
//
// Copyright: (c) 2013 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//   License: Distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#if !defined PFC_MINI_HPP
#define      PFC_MINI_HPP

// -------------------------------------------------------------------------------------------------

#if (!defined __cplusplus) || (__cplusplus < 199711L)   // use 201103L for C++11
   #error "PFC: Please use a C++98 compiler."
#endif

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <locale>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <typeinfo>

// -------------------------------------------------------------------------------------------------

#undef PFC_MINI_VERSION
#undef PFC_MINI_VERSION_MAJOR
#undef PFC_MINI_VERSION_MINOR
#undef PFC_MINI_VERSION_PATCHLEVEL

#define PFC_MINI_VERSION            "1.01"
#define PFC_MINI_VERSION_MAJOR      1
#define PFC_MINI_VERSION_MINOR      1
#define PFC_MINI_VERSION_PATCHLEVEL 904

// -------------------------------------------------------------------------------------------------

#undef PFC_COMPILER_GNU
#undef PFC_COMPILER_MICROSOFT
#undef PFC_COMPILER_MINGW
#undef PFC_COMPILER_NVCC

#if defined __CUDACC__
   #define PFC_COMPILER_NVCC
#endif

#if defined __GNUC__
   #define PFC_COMPILER_GNU

   #if defined __MINGW32__
      #define PFC_COMPILER_MINGW
   #endif
#endif

#if defined _MSC_VER
   #define PFC_COMPILER_MICROSOFT
#endif

// -------------------------------------------------------------------------------------------------

#undef  PFC_ADDR_OF
#define PFC_ADDR_OF(val) \
   pfc::addr_of (val)

#undef  PFC_ASSERT
#define PFC_ASSERT(xpr) \
   pfc::dynamic_assert ((xpr), #xpr, PFC_SRC_LOC)

#undef  PFC_DEREF
#define PFC_DEREF(ptr) \
   pfc::deref ((ptr), PFC_SRC_LOC)

#undef  PFC_DYNAMIC_ASSERT
#define PFC_DYNAMIC_ASSERT(val, xpr) \
   pfc::dynamic_assert ((val), (xpr), PFC_SRC_LOC)

#undef  PFC_NOT_NULL
#define PFC_NOT_NULL(ptr) \
   pfc::not_null ((ptr), PFC_SRC_LOC)

#undef  PFC_SAFE_DELETE
#define PFC_SAFE_DELETE(ptr) \
   pfc::safe_delete ((ptr), PFC_SRC_LOC)

#undef  PFC_SAFE_DELETE_V
#define PFC_SAFE_DELETE_V(ptr) \
   pfc::safe_delete_v ((ptr), PFC_SRC_LOC)

#undef  PFC_SIGNAL_ERROR
#define PFC_SIGNAL_ERROR(msg) \
   pfc::signal_error ((msg), PFC_SRC_LOC)

#undef  PFC_SIGNAL_WARNING
#define PFC_SIGNAL_WARNING(msg) \
   pfc::signal_warning ((msg), PFC_SRC_LOC)

#undef  PFC_SRC_LOC
#define PFC_SRC_LOC \
   pfc::src_loc (__FILE__, __LINE__)

#undef  PFC_START_HERE
#define PFC_START_HERE(fun)                                                              \
   int main () {                                                                         \
      int ret (EXIT_FAILURE);                                                            \
                                                                                         \
      try {                                                                              \
         fun (); ret = EXIT_SUCCESS;                                                     \
                                                                                         \
      } catch (pfc::exception_wrapper_base const & exc) {                                \
         std::cerr <<                                                                    \
            "\n"                                                                         \
            "PFC: Exception of type '" << exc.wrapped_typeid ().name () << "' caught.\n" \
            "PFC: " << exc.what () << '\n';                                              \
                                                                                         \
      } catch (std::exception const & exc) {                                             \
         std::cerr <<                                                                    \
            "\n"                                                                         \
            "PFC: Exception of type '" << typeid (exc).name () << "' caught.\n"          \
            "PFC: " << exc.what () << '\n';                                              \
                                                                                         \
      } catch (...) {                                                                    \
         std::cerr <<                                                                    \
            "\n"                                                                         \
            "PFC: Exception of unknown type caught.\n"                                   \
            "PFC: No hints available.\n";                                                \
      }                                                                                  \
                                                                                         \
      std::cout <<                                                                       \
         "\n"                                                                            \
         "PFC: Please input a character and press enter to continue: ";                  \
                                                                                         \
      char c; std::cin >> c; return ret;                                                 \
   }

// -------------------------------------------------------------------------------------------------

namespace pfc {

template <typename char_t> inline bool           cstr_empty    (char_t const * const p_cstr);
template <typename char_t> inline char_t const * cstr_not_null (char_t const * const p_cstr);

class src_loc {
   public:
      static src_loc const & null () {
         static src_loc const null (nullptr, 0); return null;
      }

      src_loc (char const * const p_name, int const line) : m_loc (p_name, line) {
      }

      int const & get_line () const {
         return std::get <1> (m_loc);
      }

      char const * get_name () const {
         return pfc::cstr_not_null (std::get <0> (m_loc));
      }

      bool is_null () const {
         return (get_line () <= 0) || pfc::cstr_empty (get_name ());
      }

      char const * to_string () const {
         static std::string str;

         str  = "{'";
         str += get_name ();
         str += "',";

         #if defined PFC_COMPILER_MINGW
            static std::stringstream out; out.clear (); out.str (""); out << get_line ();

            str += out.str ();
         #else
            str += std::to_string (get_line ());
         #endif

         return (str += '}').c_str ();
      }

   private:
      std::tuple <char const *, int> m_loc;
};

template <typename ostream_t> inline ostream_t & operator << (ostream_t & lhs, pfc::src_loc const & rhs) {
   return lhs << rhs.to_string ();
}

// -------------------------------------------------------------------------------------------------

class pfc_exception : public std::runtime_error {
   typedef std::runtime_error inherited;

   protected:
      explicit pfc_exception (std::string const & msg) : inherited (msg) {   // abstract class
      }
};

class assertion_exception : public pfc::pfc_exception {
   typedef pfc::pfc_exception inherited;

   public:
      explicit assertion_exception (std::string const & msg = "") : inherited (msg) {
      }
};

class callable_exception : public pfc::pfc_exception {
   typedef pfc::pfc_exception inherited;

   public:
      explicit callable_exception (std::string const & msg = "") : inherited (msg) {
      }
};

class pointer_exception : public pfc::pfc_exception {
   typedef pfc::pfc_exception inherited;

   public:
      explicit pointer_exception (std::string const & msg = "") : inherited (msg) {
      }
};

class exception_wrapper_base : public std::exception {
   typedef std::exception inherited;

   public:
      virtual std::type_info const & wrapped_typeid () const = 0;

   protected:
      exception_wrapper_base () : inherited () {   // abstract class
      }
};

template <typename T> class exception_wrapper : public pfc::exception_wrapper_base {
   typedef pfc::exception_wrapper_base inherited;

   public:
      typedef T exception_t;

      exception_wrapper (exception_t const & exc, pfc::src_loc const & loc) : inherited (), m_exc (exc), m_loc (loc) {
      }

      std::type_info const & wrapped_typeid () const {   // overridden method
         return typeid (exception_t);
      }

      char const * what () const throw () {
         static std::string str;

         str  = "Error \"";
         str += m_exc.what ();
         str += "\" occurred";

         if (!m_loc.is_null ()) {
            str += " in ";
            str += m_loc.to_string ();
         }

         return (str += '.').c_str ();
      }

   private:
      exception_t  m_exc;
      pfc::src_loc m_loc;
};

// -------------------------------------------------------------------------------------------------

template <typename char_t> inline bool cstr_empty (char_t const * const p_cstr) {
   return (p_cstr == nullptr) || (*p_cstr == char_t ());
}

template <typename char_t> inline char_t const * cstr_not_null (char_t const * const p_cstr) {
   static char_t const null = char_t (); return (p_cstr == nullptr) ? &null : p_cstr;
}

template <typename char_t> inline std::size_t cstr_size (char_t const * p_cstr) {
   std::size_t size (0);

   if (p_cstr != nullptr) {
      while (*p_cstr++ != char_t ()) {
         ++size;
      }
   }

   return size;
}

template <typename char_t> inline bool is_letter (char_t const c, std::locale const & locale = std::locale::classic ()) {
   return std::isalpha (c, locale);
}

template <typename char_t> inline bool is_alpha (char_t const c, std::locale const & locale = std::locale::classic ()) {
   return pfc::is_letter (c, locale) || (c == char_t ('_'));
}

template <typename char_t> inline bool is_digit (char_t const c, std::locale const & locale = std::locale::classic ()) {
   return std::isdigit (c, locale);
}

template <typename char_t> inline bool is_alphanum (char_t const c, std::locale const & locale = std::locale::classic ()) {
   return pfc::is_alpha (c, locale) || pfc::is_digit (c, locale);
}

template <typename char_t> inline bool is_whitespace (char_t const c, std::locale const & locale = std::locale::classic ()) {
   return std::isspace (c, locale);
}

template <typename char_t> inline std::basic_string <char_t> & tolower (std::basic_string <char_t> & str, std::locale const & locale = std::locale::classic ()) {
   std::for_each (std::begin (str), std::end (str), [&locale] (char_t & c) {
      c = std::tolower (c, locale);
   });

   return str;
}

template <typename char_t> inline std::basic_string <char_t> & toupper (std::basic_string <char_t> & str, std::locale const & locale = std::locale::classic ()) {
   std::for_each (std::begin (str), std::end (str), [&locale] (char_t & c) {
      c = std::toupper (c, locale);
   });

   return str;
}

// -------------------------------------------------------------------------------------------------

template <typename exception_t> inline void signal_error_helper (pfc::src_loc const & loc = pfc::src_loc::null ()) {
   throw pfc::exception_wrapper <exception_t> (exception_t (), loc);
}

template <typename exception_t, typename param_t> inline void signal_error_helper (param_t const & param, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   throw pfc::exception_wrapper <exception_t> (exception_t (param), loc);
}

inline void signal_error (pfc::src_loc const & loc = pfc::src_loc::null ()) {
   pfc::signal_error_helper <std::runtime_error, std::string> ("Some runtime error.", loc);
}

template <typename exception_t> inline void signal_error (pfc::src_loc const & loc = pfc::src_loc::null ()) {
   pfc::signal_error_helper <exception_t> (loc);
}

template <typename param_t> inline void signal_error (param_t const & param, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   pfc::signal_error_helper <std::runtime_error, param_t> (param, loc);
}

template <typename exception_t, typename param_t> inline void signal_error (param_t const & param, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   pfc::signal_error_helper <exception_t, param_t> (param, loc);
}

template <typename param_t> inline void signal_warning (param_t const & param, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   std::cerr << "PFC: Warning \"" << param << "\" occurred";

   if (!loc.is_null ()) {
      std::cerr << " in " << loc;
   }

   std::cerr << ".\n";
}

inline void dynamic_assert (bool const val, std::string const & xpr, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   if (!val) {
      pfc::signal_error <pfc::assertion_exception> ("The assertion '" + xpr + "' did not hold.", loc);
   }
}

// -------------------------------------------------------------------------------------------------

template <typename lvalue_t> inline lvalue_t * addr_of (lvalue_t & val) {
   return &val;
}

template <typename lvalue_t> inline lvalue_t & deref (lvalue_t * const ptr, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   if (ptr == nullptr) {
      pfc::signal_error <pfc::pointer_exception> ("An attempt to dereference a null pointer was made.", loc);
   }

   return *ptr;
}

template <typename lvalue_t> inline lvalue_t * not_null (lvalue_t * const ptr, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   if (ptr == nullptr) {
      pfc::signal_error <pfc::pointer_exception> ("'pfc::not_null' detected a null pointer.", loc);
   }

   return ptr;
}

template <typename lvalue_t> inline lvalue_t * & safe_delete (lvalue_t * & ptr, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   if (ptr == nullptr) {
      pfc::signal_warning ("An attempt to delete a null pointer was made.", loc);
   } else {
      delete ptr; ptr = nullptr;
   }

   return ptr;
}

template <typename lvalue_t> inline lvalue_t * & safe_delete_v (lvalue_t * & ptr, pfc::src_loc const & loc = pfc::src_loc::null ()) {
   if (ptr == nullptr) {
      pfc::signal_warning ("An attempt to delete a null pointer was made.", loc);
   } else {
      delete [] ptr; ptr = nullptr;
   }

   return ptr;
}

// -------------------------------------------------------------------------------------------------

typedef std::mt19937_64 default_random_engine;

template <typename gtor_t, typename value_t> inline value_t get_random_normal (value_t const m, value_t const d) {
   static_assert (
      std::is_floating_point <value_t>::value,
      "PFC: The parameters of 'pfc::get_random_normal' must be floating-point values."
   );

   static gtor_t gtor (static_cast <unsigned> (std::time (0)));

   return std::normal_distribution <value_t> (m, d) (gtor);
}

template <typename value_t> inline value_t get_random_normal (value_t const m, value_t const d) {
   return pfc::get_random_normal <pfc::default_random_engine, value_t> (m, d);
}

template <typename gtor_t, typename value_t> inline value_t get_random_uniform (value_t const l, value_t const u, gtor_t & g, std::true_type, std::false_type) {
   return std::uniform_int_distribution <value_t> (l, u) (g);
}

template <typename gtor_t, typename value_t> inline value_t get_random_uniform (value_t const l, value_t const u, gtor_t & g, std::false_type, std::true_type) {
   return std::uniform_real_distribution <value_t> (l, u) (g);
}

template <typename gtor_t, typename value_t> inline value_t get_random_uniform (value_t const l, value_t const u) {
   static_assert (
      std::is_integral <value_t>::value || std::is_floating_point <value_t>::value,
      "PFC: The parameters of 'pfc::get_random_uniform' must be integral or floating-point values."
   );

   static gtor_t gtor (static_cast <unsigned> (std::time (0)));

   return pfc::get_random_uniform <gtor_t, value_t> (l, u, gtor, std::is_integral <value_t> (), std::is_floating_point <value_t> ());
}

template <typename value_t> inline value_t get_random_uniform (value_t const l, value_t const u) {
   return pfc::get_random_uniform <pfc::default_random_engine, value_t> (l, u);
}

// -------------------------------------------------------------------------------------------------

typedef std::chrono::high_resolution_clock                                 default_timer;
typedef decltype (pfc::default_timer::now () - pfc::default_timer::now ()) default_duration;

template <typename ratio_t> /*constexpr*/ inline double ratio_value () {
   static_assert (ratio_t::num >= 0, "PFC: The numerator must be positive.");
   static_assert (ratio_t::den >  0, "PFC: The denominator must be greater than zero.");

   return 1.0 * ratio_t::num / ratio_t::den;
}

template <typename timer_t> inline auto get_timer_resolution () -> decltype (timer_t::now () - timer_t::now ()) {
   auto start (timer_t::now ());
   auto stop  (start);

   for (std::size_t r (0); r < 2; ++r) {
      start = stop;

      for (std::size_t i (0); (stop == start) && (i < 100000000); ++i) {
         stop = timer_t::now ();
      }
   }

   return stop - start;
}

inline pfc::default_duration get_timer_resolution () {
   return pfc::get_timer_resolution <default_timer> ();
}

template <typename timer_t> inline auto timed_run (std::size_t const n, std::function <void ()> const & fun) -> decltype (timer_t::now () - timer_t::now ()) {
   typedef std::remove_const <decltype (n)>::type size_t;

   if (!fun) {
      pfc::signal_error <pfc::callable_exception> ("The callable handed over to 'pfc::timed_run' has no body.");
   }

   auto const start (timer_t::now ());

   for (size_t i (0); i < n; ++i) {
      fun ();
   }

   return (timer_t::now () - start) / std::max <size_t> (1, n);
}

template <typename timer_t> inline auto timed_run (std::function <void ()> const & fun) -> decltype (timed_run <timer_t> (1, fun)) {
   return timed_run <timer_t> (1, fun);
}

inline pfc::default_duration timed_run (std::size_t const n, std::function <void ()> const & fun) {
   return timed_run <pfc::default_timer> (n, fun);
}

inline pfc::default_duration timed_run (std::function <void ()> const & fun) {
   return timed_run <pfc::default_timer> (1, fun);
}

template <typename duration_t> inline double in_s (duration_t const & duration) {
   return std::chrono::duration_cast <std::chrono::nanoseconds> (duration).count () * pfc::ratio_value <std::chrono::nanoseconds::period> ();
}

// -------------------------------------------------------------------------------------------------

typedef std::uint_fast8_t  byte_t;  static_assert (sizeof (byte_t ) == 1, "PFC: sizeof (pfc::byte_t) must be 1.");
typedef std::uint_fast32_t dword_t; static_assert (sizeof (dword_t) == 4, "PFC: sizeof (pfc::dword_t) must be 4.");
typedef std::uint16_t      word_t;  static_assert (sizeof (word_t ) == 2, "PFC: sizeof (pfc::word_t) must be 2.");

template <typename value_t> inline std::istream & read_ptr (std::istream & in, value_t * const ptr, std::size_t const count = 1) {
   typedef std::remove_reference <decltype (in)>::type::char_type char_t;   // get char type from the stream's name

   if (in.good () && (ptr != nullptr) && (count > 0)) {
      in.read (reinterpret_cast <char_t *> (ptr), count * sizeof (value_t));
   }

   return in;
}

template <typename value_t> inline std::istream & read (std::istream & in, value_t & val) {
   return pfc::read_ptr (in, &val, 1);
}

template <typename value_t> inline std::ostream & write_ptr (std::ostream & out, value_t const * const ptr, std::size_t const count = 1) {
   typedef std::remove_reference <decltype (out)>::type::char_type char_t;   // get char type from the stream's name

   if (out.good () && (ptr != nullptr) && (count > 0)) {
      out.write (reinterpret_cast <char_t const *> (ptr), count * sizeof (value_t));
   }

   return out;
}

template <typename value_t> inline std::ostream & write (std::ostream & out, value_t const & val) {
   return pfc::write_ptr (out, &val, 1);
}

// -------------------------------------------------------------------------------------------------

}   // namespace pfc

#endif   // PFC_MINI_HPP
