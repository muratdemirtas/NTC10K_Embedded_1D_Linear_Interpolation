/**
 * @file
 * Table lookup with interpolation (1-D and 2-D).
 *
 * This is a 1/2-D table lookup facility.  Each routine looks up data in a table
 * structure, interpolating as needed between data points.  The 2-D version
 * looks up along 2 axes and interpolates in two dimensions.
 *
 * <h2>Limitations</h2>
 * - The table axes (input values) must monotonically increase, or the lookup
 *   will fail.
 * - The index data type is nominally 8 bits, limiting the table length to
 *   256 elements.  Change <code>index_t</code> if larger tables are needed.
 */

#include <stdint.h>
#include <stdbool.h>
#include "lookup_1d.h"

/** Index data type */
typedef uint8_t index_t;

/**
 * 1-D table lookup.
 *
 * This function performs a 1-D table lookup with interpolation.  The output
 * value is clamped to either of the table end values when the input value is
 * out of bounds.
 *
 * @param[in]   t      table data structure
 * @param[in]   ix     input (X-axis) value
 * @param[out]  o      output data
 *
 * @retval      true   if the lookup result is suspect due to clipping
 * @retval      false  on successful lookup
 */
bool lookup1d (Table1d *t, double ix, double *o)
{
  index_t i;

  /* Within the bounds of the table */
  for (i = 0; i < t->ncols - 1; ++i)
  {
    if (   ix >= t->columns[i]
        && ix <= t->columns[i + 1])
    {
      /* Output (table) low value */
      double o_low   = t->table[i];
      /* Input (X-axis) low value */
      double i_low   = t->columns[i];
      /* Spead between the two adjacent input values */
      double i_delta = t->columns[i + 1] - t->columns[i];
      /* Spread between the two adjacent table output values */
      double o_delta = t->table[i + 1]   - t->table[i];

      /* Prevent division by zero.  We could get here if two consecutive
         input values in the table are the same. */
      if (o_delta == 0)
      {
        *o = o_low;
        return true;
      }

      *o = o_low + ((ix - i_low) * (double)o_delta) / i_delta;
      return false;
    }
  }

  /* Didn't find it (we shouldn't ever get here). */
  return true;
}


