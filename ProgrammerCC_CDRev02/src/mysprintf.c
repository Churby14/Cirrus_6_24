#include "global.h"

/**************************************************************************************************/
/**************************************************************************************************/

/* format read states */
#define DP_S_DEFAULT    0
#define DP_S_FLAGS      1
#define DP_S_MIN        2
#define DP_S_DOT        3
#define DP_S_PRECISION  4
#define DP_S_MOD        5
#define DP_S_CONV       6
#define DP_S_DONE       7

/* format flags - Bits */
#define DP_F_MINUS      (1 << 0)
#define DP_F_PLUS       (1 << 1)
#define DP_F_SPACE      (1 << 2)
#define DP_F_ZERO       (1 << 3)
#define DP_F_UP         (1 << 4)
#define DP_F_UNSIGNED   (1 << 5)
#define DP_F_PREFIX     (1 << 6)
#define DP_F_SEPARATOR  (1 << 7)
#define DP_F_FIXEDPOINT (1 << 8)

#define DP_C_SHORT      (1 << 16)
#define DP_C_LONG       (1 << 17)
#define DP_C_LLONG      (1 << 18)

#define char_to_int(p) ((p)-'0')

#ifndef MAX
#define MAX(p,q) (((p) >= (q)) ? (p) : (q))
#endif

static int currlen;
static char *buffer;

/**************************************************************************************************/
/**************************************************************************************************/
//
static int16_t my_isdigit(int16_t c) {

  return(c >= '0') && (c <= '9') ? -1 : 0;
}
/**************************************************************************************************/
/**************************************************************************************************/
//
static int16_t my_isupper(int16_t c) {

  return(c >= 'A') && (c <= 'Z') ? -1 : 0;
}
/**************************************************************************************************/
/**************************************************************************************************/
//
static int16_t my_toupper(int16_t c) {

  return(c >= 'a') && (c <= 'z') ? c & ~0x20 : c;
}
/**************************************************************************************************/
/**************************************************************************************************/
//
// Width -  Minimum width of field
// Precision - Maximum number of characters (from string) that will be placed into output buffer
//
// Examples:        TestString = "ABCDEFGHIJ"
//
//  "|%s|"          = |ABCDEFGHIJ|
//  "|%10.5s|"      = |     ABCDE|
//  "|%-10.5s|"     = |ABCDE     |
//  "|%20s|"        = |ABCDEFGHIJ          |
//  "|%-20s|"       = |          ABCDEFGHIJ|
//
static void fmtstr(char *String, uint16_t flags, int16_t Width, int16_t Precision) {

  int16_t padlen, strln; /* amount to pad */

  if ( String == NULL ) {
    String = "<NULL>";
  }
  strln = strlen(String);
  if ( Precision == -1 ) {
    Precision = strln;
  } else if ( strln > Precision ) {
    strln = Precision;
  }

  padlen = Width - strln;
  if ( padlen < 0 ) {
    padlen = 0;
  }
  if ( flags & DP_F_MINUS ) {
    padlen = -padlen; /* Left Justify */
  }

  while ( padlen > 0 ) {
    buffer[currlen++] = ' ';
    padlen--;
  }
  while ( strln > 0 ) {
    buffer[currlen++] = *String++;
    strln--;
  }
  while ( padlen < 0 ) {
    buffer[currlen++] = ' ';
    padlen++;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/

static char convert[80];

static void fmtint(int64_t value, int16_t base, int16_t Width, int16_t Precision, uint16_t flags) {

  int16_t signvalue = 0;
  uint64_t uvalue;
  int16_t place = 0;
  int16_t spadlen = 0; /* amount to space pad */
  int16_t zpadlen = 0; /* amount to zero pad */
  int16_t caps = 0;
  int16_t SeparatorCount = 0;
  int16_t FixedPointCount = 0;

  if ( Precision < 0 ) {
    Precision = 0;
  }
  uvalue = value;
  if ( !(flags & DP_F_UNSIGNED) ) {
    if ( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    } else {
      if ( flags & DP_F_PLUS ) { /* Do a sign (+/i) */
        signvalue = '+';
      } else if ( flags & DP_F_SPACE ) {
        signvalue = ' ';
      }
    }
  }

  if ( base == 2 ) {
    do {
      convert[place++] = "01"[uvalue & 1];
      uvalue >>= 1;
    } while ( uvalue != 0 );
  } else if ( base == 16 ) {
    if ( flags & DP_F_UP ) {
      caps = 1; /* Should characters be upper case? */
    }
    do {
      convert[place++] = (caps ? "0123456789ABCDEF" : "0123456789abcdef")[uvalue & 0xF];
      uvalue >>= 4;
    } while ( uvalue != 0 );
  } else {
    flags &= ~DP_F_PREFIX;                  // Make sure Prefix flag is off for base 10
    do {
      convert[place++] = "0123456789"[uvalue % 10];
      uvalue /= 10;
      if ( flags & DP_F_FIXEDPOINT ) {
        if ( ++FixedPointCount == Precision ) {
          convert[place++] = '.';
          flags &= ~DP_F_FIXEDPOINT;
        }
      } else {
        if ( flags & DP_F_SEPARATOR ) {
          if ( ++SeparatorCount == 3 ) {
            SeparatorCount = 0;
            if ( uvalue != 0 ) {
              convert[place++] = ',';
            }
          }
        }
      }
    } while ( uvalue != 0 );
    while ( flags & DP_F_FIXEDPOINT ) {
      if ( FixedPointCount++ == Precision ) {
        convert[place++] = '.';
        flags &= ~DP_F_FIXEDPOINT;
      } else
        convert[place++] = '0';
    }
    if ( (FixedPointCount != 0) && (convert[place - 1] == '.') ) {
      convert[place++] = '0';
    }
  }
  convert[place] = '\0';

  zpadlen = Precision - place;
  spadlen = Width - MAX(Precision, place);
  if ( flags & DP_F_PREFIX ) {
    spadlen -= 2;
  }
  if ( signvalue ) {
    spadlen -= 1;
  }

  if ( zpadlen < 0 ) {
    zpadlen = 0;
  }
  if ( spadlen < 0 ) {
    spadlen = 0;
  }
  if ( flags & DP_F_ZERO ) {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if ( flags & DP_F_MINUS ) {
    spadlen = -spadlen; /* Left Justifty */
  }

  /* Spaces */
  while ( spadlen > 0 ) {
    buffer[currlen++] = ' ';
    --spadlen;
  }

  /* Sign */
  if ( signvalue ) {
    buffer[currlen++] = signvalue;
  }

  if ( flags & DP_F_PREFIX ) {
    buffer[currlen++] = '0';
    if ( base == 16 ) {
      buffer[currlen++] = 'x';
    } else {
      buffer[currlen++] = 'b';
    }
  }

  /* Zeros */
  while ( zpadlen > 0 ) {
    buffer[currlen++] = '0';
    --zpadlen;
  }

  /* Digits */
  while ( place > 0 ) {
    buffer[currlen++] = convert[--place];
  }

  /* Left Justified spaces */
  while ( spadlen < 0 ) {
    buffer[currlen++] = ' ';
    ++spadlen;
  }
}

/**************************************************************************************************/
/**************************************************************************************************/

static int16_t dopr(char *strbuffer, const char *format, va_list args) {

  char ch;
  char *strvalue;
  int16_t Width, Precision, state, base;
  uint16_t flags;
  int64_t value;

  Width = 0;
  Precision = -1;
  flags = 0;
  currlen = 0;
  buffer = strbuffer;
  state = DP_S_DEFAULT;

  ch = *format++;
  while ( state != DP_S_DONE ) {
    if ( ch == '\0' ) {
      state = DP_S_DONE;              // End of Format String
    }
    switch ( state ) {
    case DP_S_DEFAULT:
      if ( ch == '%' ) {                          // Look for '%'
        Width = 0;
        Precision = -1;
        flags = 0;
        state = DP_S_FLAGS;
      } else
        buffer[currlen++] = ch;
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch ( ch ) {
      case '-':
        flags |= DP_F_MINUS;            // Left Justify
        ch = *format++;
        break;
      case '+':
        flags |= DP_F_PLUS;             // Force a '+' for all positive values
        ch = *format++;
        break;
      case ' ':
        flags |= DP_F_SPACE;            // Forse a ' ' for all positive values
        ch = *format++;
        break;
      case '0':
        flags |= DP_F_ZERO;             // Force value to be printed with leading zeros
        ch = *format++;
        break;
      case '#':
        flags |= DP_F_PREFIX;           // Force value to be printed with leading zeros
        ch = *format++;
        break;
      case '\'':
        flags |= DP_F_SEPARATOR;        // Force value to be printed with leading zeros
        ch = *format++;
        break;
      default:
        if ( flags & DP_F_MINUS ) {
          flags &= ~DP_F_ZERO;
        }
        state = DP_S_MIN;
        break;
      }
      break;
    case DP_S_MIN:
      if ( my_isdigit((unsigned char) ch) ) {             // Look for a minimum width
        Width = (10 * Width) + char_to_int(ch);
        ch = *format++;
      } else if ( ch == '*' ) {                         // Get minimum width parameter from argument list
        Width = va_arg(args, int);
        ch = *format++;
        state = DP_S_DOT;
      } else
        state = DP_S_DOT;
      break;
    case DP_S_DOT:
      if ( ch == '.' ) {                              // if there is a period, the maximum width is next
        state = DP_S_PRECISION;
        ch = *format++;
        if ( ch == '.' ) {                              // if there is a period, the maximum width is next
          flags |= DP_F_FIXEDPOINT;
          ch = *format++;
        }
      } else
        state = DP_S_MOD;
      break;
    case DP_S_PRECISION:
      if ( my_isdigit((unsigned char) ch) ) {             // Get maximum width
        if ( Precision < 0 ) {
          Precision = 0;
          flags &= ~DP_F_ZERO;
        }
        Precision = (10 * Precision) + char_to_int(ch);
        ch = *format++;
      } else if ( ch == '*' ) {                         // Get maximum width parameter from aryument list
        Precision = va_arg(args, int);
        ch = *format++;
        flags &= ~DP_F_ZERO;
        state = DP_S_MOD;
      } else {
        state = DP_S_MOD;
        if ( Precision == 0 ) {
          flags &= ~DP_F_FIXEDPOINT;
        }
      }
      break;
    case DP_S_MOD:
      if ( ch == 'h' ) {                              // 16-bit value
        flags |= DP_C_SHORT;
        ch = *format++;
      } else if ( ch == 'l' ) {                         // 32-bit value (%u and %lu are the same on the ARM7 processor)
        flags |= DP_C_LONG;                     // This flag isn't used - as 32-bits is the default for the ARM7
        ch = *format++;
        if ( ch == 'l' ) {
          flags |= DP_C_LLONG;
          ch = *format++;
        }
      } else if ( ch == 'L' ) {                         // 64-bit value
        flags |= DP_C_LLONG;
        ch = *format++;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      if ( my_isupper(ch) ) {
        flags |= DP_F_UP;
      }
      ch = my_toupper(ch);
      if ( (ch == 'D') || (ch == 'I') ) {
        if ( flags & DP_C_SHORT ) {
          value = (short) (va_arg (args, int) & 0xFFFF);
        } else if ( flags & DP_C_LLONG ) {
          value = va_arg(args, long long);
        } else {
          value = (int) (va_arg (args, int) & 0xFFFFFFFF);
        }
        fmtint(value, 10, Width, Precision, flags);
      } else if ( (ch == 'B') || (ch == 'U') || (ch == 'X') || (ch == 'P') ) {
        flags |= DP_F_UNSIGNED;
        if ( flags & DP_C_SHORT ) {
          value = (unsigned short) va_arg(args, int) & 0xFFFF;
        } else if ( flags & DP_C_LLONG ) {
          value = va_arg(args, long long);
        } else {
          value = (unsigned int) va_arg(args, int) & 0xFFFFFFFF;
        }
        if ( ch == 'B' ) {
          base = 2;
        } else if ( ch == 'U' ) {
          base = 10;
        } else {
          base = 16;
        }
        fmtint(value, base, Width, Precision, flags);
      } else if ( ch == 'C' ) {
        buffer[currlen++] = va_arg(args, int);
      } else if ( ch == 'S' ) {
        strvalue = va_arg(args, char *);
        fmtstr(strvalue, flags, Width, Precision);
      } else if ( ch == '%' ) {
        buffer[currlen++] = '%';
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      break;
    case DP_S_DONE:
      break;
    default:
      break;
    }
  }
  buffer[currlen] = '\0';
  return currlen;
}

/**************************************************************************************************/
/**************************************************************************************************/

int16_t mysprintf(void *str, const void *fmt, ...) {

  int16_t ret;
  va_list ap;

  va_start(ap, fmt);
  ret = dopr(str, fmt, ap);
  va_end(ap);
  return ret;
}

