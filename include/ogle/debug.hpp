#ifndef OGLE_DEBUG_HPP
#define OGLE_DEBUG_HPP

#ifdef DEBUG_LOG
#define DEBUG(X) do { std::cerr << "DEBUG: " << X << '\n'; } while (0)
#else
#define DEBUG(X)
#endif

#endif // OGLE_DEBUG_HPP
