#ifndef PID_EXPORTS_H
#define PID_EXPORTS_H

typedef struct {
  double p;
  double i;
  double d;
} PID_t;

typedef enum {
  XY_P,
  XY_I,
  XY_D,
  Z_P,
  Z_I,
  Z_D,
} PID__field_id_t;

typedef struct {
  PID__field_id_t pid_field_id;
  double value;
} PID__set_cnc_t;

void PID__update_field(PID__field_id_t field_id, double value);

#endif /* PID_EXPORTS_H */
