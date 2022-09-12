
add_compile_definitions(GTPSA_AS_LIB)

set(gtpsa_HEADERS
  ${mad_ng_SRC_DIR}/mad_bit.h
  ${mad_ng_SRC_DIR}/mad_cst.h
  ${mad_ng_SRC_DIR}/mad_ctpsa.h
  ${mad_ng_SRC_DIR}/mad_ctpsa_impl.h
  ${mad_ng_SRC_DIR}/mad_def.h
  ${mad_ng_SRC_DIR}/mad_desc.h
  ${mad_ng_SRC_DIR}/mad_desc_impl.h
  ${mad_ng_SRC_DIR}/mad_erfw.h
  ${mad_ng_SRC_DIR}/mad_log.h
  ${mad_ng_SRC_DIR}/mad_mat.h
  ${mad_ng_SRC_DIR}/mad_mem.h
  ${mad_ng_SRC_DIR}/mad_mono.h
  ${mad_ng_SRC_DIR}/mad_num.h
  ${mad_ng_SRC_DIR}/mad_poly.h
  ${mad_ng_SRC_DIR}/mad_rad.h
  ${mad_ng_SRC_DIR}/mad_str.h
  ${mad_ng_SRC_DIR}/mad_tpsa.h
  ${mad_ng_SRC_DIR}/mad_tpsa_impl.h
  ${mad_ng_SRC_DIR}/mad_vec.h
  ${mad_ng_SRC_DIR}/mad_ver.h
)

set(gtpsa_FILES
  ${mad_ng_SRC_DIR}/libgtpsa/mad_log.c
  ${mad_ng_SRC_DIR}/mad_bit.c
  ${mad_ng_SRC_DIR}/mad_cst.c
  ${mad_ng_SRC_DIR}/mad_ctpsa.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_comp.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_conv.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_fun.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_io.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_minv.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_mops.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_ops.c
  ${mad_ng_SRC_DIR}/mad_desc.c
  ${mad_ng_SRC_DIR}/mad_erfw.c
  ${mad_ng_SRC_DIR}/mad_mat.c
  ${mad_ng_SRC_DIR}/mad_mem.c
  ${mad_ng_SRC_DIR}/mad_mono.c
  ${mad_ng_SRC_DIR}/mad_num.c
  ${mad_ng_SRC_DIR}/mad_poly.c
  ${mad_ng_SRC_DIR}/mad_rad.c
  ${mad_ng_SRC_DIR}/mad_str.c
  ${mad_ng_SRC_DIR}/mad_tpsa.c
  ${mad_ng_SRC_DIR}/mad_tpsa_comp.c
  ${mad_ng_SRC_DIR}/mad_tpsa_fun.c
  ${mad_ng_SRC_DIR}/mad_tpsa_io.c
  ${mad_ng_SRC_DIR}/mad_tpsa_minv.c
  ${mad_ng_SRC_DIR}/mad_tpsa_mops.c
  ${mad_ng_SRC_DIR}/mad_tpsa_ops.c
  ${mad_ng_SRC_DIR}/mad_vec.c
  )


add_library(gtpsa SHARED
  ${gtpsa_FILES}
  ${gtpsa_HEADERS}
)
