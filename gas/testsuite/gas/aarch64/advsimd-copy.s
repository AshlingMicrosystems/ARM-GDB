	dup	v0.8b, v0.b[0]
	dup	v31.8b, v0.b[0]
	dup	v0.8b, v31.b[0]
	dup	v0.8b, v0.b[15]
	dup	v0.4h, v0.h[0]
	dup	v31.4h, v0.h[0]
	dup	v0.4h, v31.h[0]
	dup	v0.4h, v0.h[7]
	dup	v0.2s, v0.s[0]
	dup	v31.2s, v0.s[0]
	dup	v0.2s, v31.s[0]
	dup	v0.2s, v0.s[3]
	dup	v0.16b, v0.b[0]
	dup	v31.16b, v0.b[0]
	dup	v0.16b, v31.b[0]
	dup	v0.16b, v0.b[15]
	dup	v0.8h, v0.h[0]
	dup	v31.8h, v0.h[0]
	dup	v0.8h, v31.h[0]
	dup	v0.8h, v0.h[7]
	dup	v0.4s, v0.s[0]
	dup	v31.4s, v0.s[0]
	dup	v0.4s, v31.s[0]
	dup	v0.4s, v0.s[3]
	dup	v0.2d, v0.d[0]
	dup	v31.2d, v0.d[0]
	dup	v0.2d, v31.d[0]
	dup	v0.2d, v0.d[1]

	dup	v0.8b, w0
	dup	v31.8b, w0
	dup	v0.8b, wzr
	dup	v0.4h, w0
	dup	v31.4h, w0
	dup	v0.4h, wzr
	dup	v0.2s, w0
	dup	v31.2s, w0
	dup	v0.2s, wzr
	dup	v0.16b, w0
	dup	v31.16b, w0
	dup	v0.16b, wzr
	dup	v0.8h, w0
	dup	v31.8h, w0
	dup	v0.8h, wzr
	dup	v0.4s, w0
	dup	v31.4s, w0
	dup	v0.4s, wzr
	dup	v0.2d, x0
	dup	v31.2d, x0
	dup	v0.2d, xzr
// Unspecified bits in imm5 (20..16) are ignored but should be set to zero by
// an assembler.  This tests disassembly when the ignored bits are nonzero.
	.inst	0x0e150c00
	.inst	0x4e180c00

	smov	w0, v0.b[0]
	smov	wzr, v0.b[0]
	smov	w0, v31.b[0]
	smov	w0, v0.b[15]
	smov	w0, v0.h[0]
	smov	wzr, v0.h[0]
	smov	w0, v31.h[0]
	smov	w0, v0.h[7]
	smov	x0, v0.b[0]
	smov	xzr, v0.b[0]
	smov	x0, v31.b[0]
	smov	x0, v0.b[15]
	smov	x0, v0.h[0]
	smov	xzr, v0.h[0]
	smov	x0, v31.h[0]
	smov	x0, v0.h[7]
	smov	x0, v0.s[0]
	smov	xzr, v0.s[0]
	smov	x0, v31.s[0]
	smov	x0, v0.s[3]

	umov	w0, v0.b[0]
	umov	wzr, v0.b[0]
	umov	w0, v31.b[0]
	umov	w0, v0.b[15]
	umov	w0, v0.h[0]
	umov	wzr, v0.h[0]
	umov	w0, v31.h[0]
	umov	w0, v0.h[7]
	umov	w0, v0.s[0]
	umov	wzr, v0.s[0]
	umov	w0, v31.s[0]
	umov	w0, v0.s[3]
	umov	x0, v0.d[0]
	umov	xzr, v0.d[0]
	umov	x0, v31.d[0]
	umov	x0, v0.d[1]

	mov	w0, v0.s[0]
	mov	wzr, v0.s[0]
	mov	w0, v31.s[0]
	mov	w0, v0.s[3]
	mov	x0, v0.d[0]
	mov	xzr, v0.d[0]
	mov	x0, v31.d[0]
	mov	x0, v0.d[1]

	ins	v0.b[0], w0
	ins	v31.b[0], w0
	ins	v0.b[0], wzr
	ins	v0.b[15], w0
	ins	v0.h[0], w0
	ins	v31.h[0], w0
	ins	v0.h[0], wzr
	ins	v0.h[7], w0
	ins	v0.s[0], w0
	ins	v31.s[0], w0
	ins	v0.s[0], wzr
	ins	v0.s[3], w0
	ins	v0.d[0], x0
	ins	v31.d[0], x0
	ins	v0.d[0], xzr
	ins	v0.d[1], x0

	mov	v0.b[0], w0
	mov	v31.b[0], w0
	mov	v0.b[0], wzr
	mov	v0.b[15], w0
	mov	v0.h[0], w0
	mov	v31.h[0], w0
	mov	v0.h[0], wzr
	mov	v0.h[7], w0
	mov	v0.s[0], w0
	mov	v31.s[0], w0
	mov	v0.s[0], wzr
	mov	v0.s[3], w0
	mov	v0.d[0], x0
	mov	v31.d[0], x0
	mov	v0.d[0], xzr
	mov	v0.d[1], x0

	ins	v0.b[0], v0.b[0]
	ins	v31.b[0], v0.b[0]
	ins	v0.b[0], v31.b[0]
	ins	v0.b[15], v0.b[0]
	ins	v0.b[0], v0.b[15]
	ins	v0.h[0], v0.h[0]
	ins	v31.h[0], v0.h[0]
	ins	v0.h[0], v31.h[0]
	ins	v0.h[7], v0.h[0]
	ins	v0.h[0], v0.h[7]
	ins	v0.s[0], v0.s[0]
	ins	v31.s[0], v0.s[0]
	ins	v0.s[0], v31.s[0]
	ins	v0.s[3], v0.s[0]
	ins	v0.s[0], v0.s[3]
	ins	v0.d[0], v0.d[0]
	ins	v31.d[0], v0.d[0]
	ins	v0.d[0], v31.d[0]
	ins	v0.d[1], v0.d[0]
	ins	v0.d[0], v0.d[1]
// Unspecified bits in imm4 (14..11) are ignored but should be set to zero by
// an assembler.  This tests disassembly when the ignored bits are nonzero.
	.inst	0x6e022c00
	.inst	0x6e083c00

	mov	v0.b[0], v0.b[0]
	mov	v31.b[0], v0.b[0]
	mov	v0.b[0], v31.b[0]
	mov	v0.b[15], v0.b[0]
	mov	v0.b[0], v0.b[15]
	mov	v0.h[0], v0.h[0]
	mov	v31.h[0], v0.h[0]
	mov	v0.h[0], v31.h[0]
	mov	v0.h[7], v0.h[0]
	mov	v0.h[0], v0.h[7]
	mov	v0.s[0], v0.s[0]
	mov	v31.s[0], v0.s[0]
	mov	v0.s[0], v31.s[0]
	mov	v0.s[3], v0.s[0]
	mov	v0.s[0], v0.s[3]
	mov	v0.d[0], v0.d[0]
	mov	v31.d[0], v0.d[0]
	mov	v0.d[0], v31.d[0]
	mov	v0.d[1], v0.d[0]
	mov	v0.d[0], v0.d[1]
