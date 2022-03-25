#ifndef LOOKUP_1D_H_
#define LOOKUP_1D_H_

/**
 * @file
 * Table lookup with interpolation (1-D and 2-D) header file.
 */

#include <stdbool.h>

typedef struct{
	double celcius;
	double resistance;
}ts_ntc_interp1;

/** One dimensional lookup table. */
typedef struct
{
  /** Number of elements in the table.  This must be at least 2. */
  unsigned int ncols;
  /** List of input values. */
  double columns[100];
  /** Table data (output values).  The output values list must have the same
      length as the input list. */
  double table[100];
} Table1d;

/* Prototypes */
bool lookup1d (Table1d *t, double ix, double *o);

#endif /* LOOKUP_1D_H_ */
