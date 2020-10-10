#ifndef __DETOURS_GL_DEF_H__
#define __DETOURS_GL_DEF_H__
#include "GLES2/gl2.h"
#include "EGL/egl.h"
void         GL_APIENTRY DetourglActiveTexture (GLenum texture);
void         GL_APIENTRY DetourglAttachShader (GLuint program, GLuint shader);
void         GL_APIENTRY DetourglBindAttribLocation (GLuint program, GLuint index, const char* name);
void         GL_APIENTRY DetourglBindFramebuffer (GLenum target, GLuint framebuffer);
void         GL_APIENTRY DetourglBindRenderbuffer (GLenum target, GLuint renderbuffer);
void         GL_APIENTRY DetourglBindTexture (GLenum target, GLuint texture);
void         GL_APIENTRY DetourglBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void         GL_APIENTRY DetourglBlendEquation( GLenum mode );
void         GL_APIENTRY DetourglBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
void         GL_APIENTRY DetourglBlendFunc (GLenum sfactor, GLenum dfactor);
void         GL_APIENTRY DetourglBlendFuncSeparate (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
GLenum       GL_APIENTRY DetourglCheckFramebufferStatus (GLenum target);
void         GL_APIENTRY DetourglClear (GLbitfield mask);
void         GL_APIENTRY DetourglClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void         GL_APIENTRY DetourglClearDepthf (GLclampf depth);
void         GL_APIENTRY DetourglClearStencil (GLint s);
void         GL_APIENTRY DetourglColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void         GL_APIENTRY DetourglCompileShader (GLuint shader);
void         GL_APIENTRY DetourglCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
void         GL_APIENTRY DetourglCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void         GL_APIENTRY DetourglCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void         GL_APIENTRY DetourglCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLuint       GL_APIENTRY DetourglCreateProgram (void);
GLuint       GL_APIENTRY DetourglCreateShader (GLenum type);
void         GL_APIENTRY DetourglCullFace (GLenum mode);
void         GL_APIENTRY DetourglDeleteBuffers (GLsizei n, const GLuint* buffers);
void         GL_APIENTRY DetourglDeleteFramebuffers (GLsizei n, const GLuint* framebuffers);
void         GL_APIENTRY DetourglDeleteTextures (GLsizei n, const GLuint* textures);
void         GL_APIENTRY DetourglDeleteProgram (GLuint program);
void         GL_APIENTRY DetourglDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers);
void         GL_APIENTRY DetourglDeleteShader (GLuint shader);
void         GL_APIENTRY DetourglDetachShader (GLuint program, GLuint shader);
void         GL_APIENTRY DetourglDepthFunc (GLenum func);
void         GL_APIENTRY DetourglDepthMask (GLboolean flag);
void         GL_APIENTRY DetourglDepthRangef (GLclampf zNear, GLclampf zFar);
void         GL_APIENTRY DetourglDisable (GLenum cap);
void         GL_APIENTRY DetourglDisableVertexAttribArray (GLuint index);
void         GL_APIENTRY DetourglDrawArrays (GLenum mode, GLint first, GLsizei count);
void         GL_APIENTRY DetourglDrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices);
void         GL_APIENTRY DetourglDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
void         GL_APIENTRY DetourglEnable (GLenum cap);
void         GL_APIENTRY DetourglEnableVertexAttribArray (GLuint index);
void         GL_APIENTRY DetourglFinish (void);
void         GL_APIENTRY DetourglFlush (void);
void         GL_APIENTRY DetourglFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void         GL_APIENTRY DetourglFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void         GL_APIENTRY DetourglFrontFace (GLenum mode);
void         GL_APIENTRY DetourglGenBuffers (GLsizei n, GLuint* buffers);
void         GL_APIENTRY DetourglGenerateMipmap (GLenum target);
void         GL_APIENTRY DetourglGenFramebuffers (GLsizei n, GLuint* framebuffers);
void         GL_APIENTRY DetourglGenRenderbuffers (GLsizei n, GLuint* renderbuffers);
void         GL_APIENTRY DetourglGenTextures (GLsizei n, GLuint* textures);
void         GL_APIENTRY DetourglGetActiveAttrib (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
void         GL_APIENTRY DetourglGetActiveUniform (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
void         GL_APIENTRY DetourglGetAttachedShaders (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
int          GL_APIENTRY DetourglGetAttribLocation (GLuint program, const char* name);
void         GL_APIENTRY DetourglGetBooleanv (GLenum pname, GLboolean* params);
void         GL_APIENTRY DetourglGetBufferParameteriv (GLenum target, GLenum pname, GLint* params);
GLenum       GL_APIENTRY DetourglGetError (void);
void         GL_APIENTRY DetourglGetFloatv (GLenum pname, GLfloat* params);
void         GL_APIENTRY DetourglGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetIntegerv (GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetProgramiv (GLuint program, GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetProgramInfoLog (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
void         GL_APIENTRY DetourglGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetShaderiv (GLuint shader, GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetShaderInfoLog (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
void         GL_APIENTRY DetourglGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
void         GL_APIENTRY DetourglGetShaderSource (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
const GLubyte* GL_APIENTRY DetourglGetString (GLenum name);
void         GL_APIENTRY DetourglGetTexParameterfv (GLenum target, GLenum pname, GLfloat* params);
void         GL_APIENTRY DetourglGetTexParameteriv (GLenum target, GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetUniformfv (GLuint program, GLint location, GLfloat* params);
void         GL_APIENTRY DetourglGetUniformiv (GLuint program, GLint location, GLint* params);
int          GL_APIENTRY DetourglGetUniformLocation (GLuint program, const char* name);
void         GL_APIENTRY DetourglGetVertexAttribfv (GLuint index, GLenum pname, GLfloat* params);
void         GL_APIENTRY DetourglGetVertexAttribiv (GLuint index, GLenum pname, GLint* params);
void         GL_APIENTRY DetourglGetVertexAttribPointerv (GLuint index, GLenum pname, void** pointer);
void         GL_APIENTRY DetourglHint (GLenum target, GLenum mode);
GLboolean    GL_APIENTRY DetourglIsBuffer (GLuint buffer);
GLboolean    GL_APIENTRY DetourglIsEnabled (GLenum cap);
GLboolean    GL_APIENTRY DetourglIsFramebuffer (GLuint framebuffer);
GLboolean    GL_APIENTRY DetourglIsProgram (GLuint program);
GLboolean    GL_APIENTRY DetourglIsRenderbuffer (GLuint renderbuffer);
GLboolean    GL_APIENTRY DetourglIsShader (GLuint shader);
GLboolean    GL_APIENTRY DetourglIsTexture (GLuint texture);
void         GL_APIENTRY DetourglLineWidth (GLfloat width);
void         GL_APIENTRY DetourglLinkProgram (GLuint program);
void         GL_APIENTRY DetourglPixelStorei (GLenum pname, GLint param);
void         GL_APIENTRY DetourglPolygonOffset (GLfloat factor, GLfloat units);
void         GL_APIENTRY DetourglReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
void         GL_APIENTRY DetourglReleaseShaderCompiler (void);
void         GL_APIENTRY DetourglRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void         GL_APIENTRY DetourglSampleCoverage (GLclampf value, GLboolean invert);
void         GL_APIENTRY DetourglScissor (GLint x, GLint y, GLsizei width, GLsizei height);
void         GL_APIENTRY DetourglShaderBinary (GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
void         GL_APIENTRY DetourglShaderSource (GLuint shader, GLsizei count, const char** string, const GLint* length);
void         GL_APIENTRY DetourglStencilFunc (GLenum func, GLint ref, GLuint mask);
void         GL_APIENTRY DetourglStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask);
void         GL_APIENTRY DetourglStencilMask (GLuint mask);
void         GL_APIENTRY DetourglStencilMaskSeparate (GLenum face, GLuint mask);
void         GL_APIENTRY DetourglStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
void         GL_APIENTRY DetourglStencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
void         GL_APIENTRY DetourglTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
void         GL_APIENTRY DetourglTexParameterf (GLenum target, GLenum pname, GLfloat param);
void         GL_APIENTRY DetourglTexParameterfv (GLenum target, GLenum pname, const GLfloat* params);
void         GL_APIENTRY DetourglTexParameteri (GLenum target, GLenum pname, GLint param);
void         GL_APIENTRY DetourglTexParameteriv (GLenum target, GLenum pname, const GLint* params);
void         GL_APIENTRY DetourglTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void         GL_APIENTRY DetourglUniform1f (GLint location, GLfloat x);
void         GL_APIENTRY DetourglUniform1fv (GLint location, GLsizei count, const GLfloat* v);
void         GL_APIENTRY DetourglUniform1i (GLint location, GLint x);
void         GL_APIENTRY DetourglUniform1iv (GLint location, GLsizei count, const GLint* v);
void         GL_APIENTRY DetourglUniform2f (GLint location, GLfloat x, GLfloat y);
void         GL_APIENTRY DetourglUniform2fv (GLint location, GLsizei count, const GLfloat* v);
void         GL_APIENTRY DetourglUniform2i (GLint location, GLint x, GLint y);
void         GL_APIENTRY DetourglUniform2iv (GLint location, GLsizei count, const GLint* v);
void         GL_APIENTRY DetourglUniform3f (GLint location, GLfloat x, GLfloat y, GLfloat z);
void         GL_APIENTRY DetourglUniform3fv (GLint location, GLsizei count, const GLfloat* v);
void         GL_APIENTRY DetourglUniform3i (GLint location, GLint x, GLint y, GLint z);
void         GL_APIENTRY DetourglUniform3iv (GLint location, GLsizei count, const GLint* v);
void         GL_APIENTRY DetourglUniform4f (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void         GL_APIENTRY DetourglUniform4fv (GLint location, GLsizei count, const GLfloat* v);
void         GL_APIENTRY DetourglUniform4i (GLint location, GLint x, GLint y, GLint z, GLint w);
void         GL_APIENTRY DetourglUniform4iv (GLint location, GLsizei count, const GLint* v);
void         GL_APIENTRY DetourglUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void         GL_APIENTRY DetourglUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void         GL_APIENTRY DetourglUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void         GL_APIENTRY DetourglUseProgram (GLuint program);
void         GL_APIENTRY DetourglValidateProgram (GLuint program);
void         GL_APIENTRY DetourglVertexAttrib1f (GLuint indx, GLfloat x);
void         GL_APIENTRY DetourglVertexAttrib1fv (GLuint indx, const GLfloat* values);
void         GL_APIENTRY DetourglVertexAttrib2f (GLuint indx, GLfloat x, GLfloat y);
void         GL_APIENTRY DetourglVertexAttrib2fv (GLuint indx, const GLfloat* values);
void         GL_APIENTRY DetourglVertexAttrib3f (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
void         GL_APIENTRY DetourglVertexAttrib3fv (GLuint indx, const GLfloat* values);
void         GL_APIENTRY DetourglVertexAttrib4f (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void         GL_APIENTRY DetourglVertexAttrib4fv (GLuint indx, const GLfloat* values);
void         GL_APIENTRY DetourglVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
void         GL_APIENTRY DetourglViewport (GLint x, GLint y, GLsizei width, GLsizei height);
void         GL_APIENTRY DetourglBufferData (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
void         GL_APIENTRY DetourglBindBuffer (GLenum target, GLuint buffer);
void         GL_APIENTRY DetourglBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
void *		 GL_APIENTRY glMapBufferOES(GLenum target, GLenum access);
void *GL_APIENTRY glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean	 GL_APIENTRY glUnmapBufferOES(GLenum target);
GLboolean	 GL_APIENTRY glUnmapBuffer(GLenum target);
typedef void         (GL_APIENTRY *pfnglActiveTexture) (GLenum texture);
typedef void         (GL_APIENTRY *pfnglAttachShader) (GLuint program, GLuint shader);
typedef void         (GL_APIENTRY *pfnglBindAttribLocation) (GLuint program, GLuint index, const char* name);
typedef void         (GL_APIENTRY *pfnglBindFramebuffer) (GLenum target, GLuint framebuffer);
typedef void         (GL_APIENTRY *pfnglBindRenderbuffer) (GLenum target, GLuint renderbuffer);
typedef void         (GL_APIENTRY *pfnglBindTexture) (GLenum target, GLuint texture);
typedef void         (GL_APIENTRY *pfnglBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void         (GL_APIENTRY *pfnglBlendEquation)( GLenum mode );
typedef void         (GL_APIENTRY *pfnglBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
typedef void         (GL_APIENTRY *pfnglBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void         (GL_APIENTRY *pfnglBlendFuncSeparate) (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef GLenum       (GL_APIENTRY *pfnglCheckFramebufferStatus) (GLenum target);
typedef void         (GL_APIENTRY *pfnglClear) (GLbitfield mask);
typedef void         (GL_APIENTRY *pfnglClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void         (GL_APIENTRY *pfnglClearDepthf) (GLclampf depth);
typedef void         (GL_APIENTRY *pfnglClearStencil) (GLint s);
typedef void         (GL_APIENTRY *pfnglColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void         (GL_APIENTRY *pfnglCompileShader) (GLuint shader);
typedef void         (GL_APIENTRY *pfnglCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void         (GL_APIENTRY *pfnglCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void         (GL_APIENTRY *pfnglCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void         (GL_APIENTRY *pfnglCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint       (GL_APIENTRY *pfnglCreateProgram) (void);
typedef GLuint       (GL_APIENTRY *pfnglCreateShader) (GLenum type);
typedef void         (GL_APIENTRY *pfnglCullFace) (GLenum mode);
typedef void         (GL_APIENTRY *pfnglDeleteBuffers) (GLsizei n, const GLuint* buffers);
typedef void         (GL_APIENTRY *pfnglDeleteFramebuffers) (GLsizei n, const GLuint* framebuffers);
typedef void         (GL_APIENTRY *pfnglDeleteTextures) (GLsizei n, const GLuint* textures);
typedef void         (GL_APIENTRY *pfnglDeleteProgram) (GLuint program);
typedef void         (GL_APIENTRY *pfnglDeleteRenderbuffers) (GLsizei n, const GLuint* renderbuffers);
typedef void         (GL_APIENTRY *pfnglDeleteShader) (GLuint shader);
typedef void         (GL_APIENTRY *pfnglDetachShader) (GLuint program, GLuint shader);
typedef void         (GL_APIENTRY *pfnglDepthFunc) (GLenum func);
typedef void         (GL_APIENTRY *pfnglDepthMask) (GLboolean flag);
typedef void         (GL_APIENTRY *pfnglDepthRangef) (GLclampf zNear, GLclampf zFar);
typedef void         (GL_APIENTRY *pfnglDisable) (GLenum cap);
typedef void         (GL_APIENTRY *pfnglDisableVertexAttribArray) (GLuint index);
typedef void         (GL_APIENTRY *pfnglDrawArrays) (GLenum mode, GLint first, GLsizei count);
typedef void         (GL_APIENTRY *pfnglDrawElements) (GLenum mode, GLsizei count, GLenum type, const void* indices);
typedef void         (GL_APIENTRY *pfnglDrawElementsInstanced) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void         (GL_APIENTRY *pfnglEnable) (GLenum cap);
typedef void         (GL_APIENTRY *pfnglEnableVertexAttribArray) (GLuint index);
typedef void         (GL_APIENTRY *pfnglFinish) (void);
typedef void         (GL_APIENTRY *pfnglFlush) (void);
typedef void         (GL_APIENTRY *pfnglFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void         (GL_APIENTRY *pfnglFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void         (GL_APIENTRY *pfnglFrontFace) (GLenum mode);
typedef void         (GL_APIENTRY *pfnglGenBuffers) (GLsizei n, GLuint* buffers);
typedef void         (GL_APIENTRY *pfnglGenerateMipmap) (GLenum target);
typedef void         (GL_APIENTRY *pfnglGenFramebuffers) (GLsizei n, GLuint* framebuffers);
typedef void         (GL_APIENTRY *pfnglGenRenderbuffers) (GLsizei n, GLuint* renderbuffers);
typedef void         (GL_APIENTRY *pfnglGenTextures) (GLsizei n, GLuint* textures);
typedef void         (GL_APIENTRY *pfnglGetActiveAttrib) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef void         (GL_APIENTRY *pfnglGetActiveUniform) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef void         (GL_APIENTRY *pfnglGetAttachedShaders) (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
typedef int          (GL_APIENTRY *pfnglGetAttribLocation) (GLuint program, const char* name);
typedef void         (GL_APIENTRY *pfnglGetBooleanv) (GLenum pname, GLboolean* params);
typedef void         (GL_APIENTRY *pfnglGetBufferParameteriv) (GLenum target, GLenum pname, GLint* params);
typedef GLenum       (GL_APIENTRY *pfnglGetError) (void);
typedef void         (GL_APIENTRY *pfnglGetFloatv) (GLenum pname, GLfloat* params);
typedef void         (GL_APIENTRY *pfnglGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetIntegerv) (GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetProgramiv) (GLuint program, GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetProgramInfoLog) (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
typedef void         (GL_APIENTRY *pfnglGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetShaderiv) (GLuint shader, GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetShaderInfoLog) (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
typedef void         (GL_APIENTRY *pfnglGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
typedef void         (GL_APIENTRY *pfnglGetShaderSource) (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
typedef void         (GL_APIENTRY *pfnglGetTexParameterfv) (GLenum target, GLenum pname, GLfloat* params);
typedef void         (GL_APIENTRY *pfnglGetTexParameteriv) (GLenum target, GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetUniformfv) (GLuint program, GLint location, GLfloat* params);
typedef void         (GL_APIENTRY *pfnglGetUniformiv) (GLuint program, GLint location, GLint* params);
typedef int          (GL_APIENTRY *pfnglGetUniformLocation) (GLuint program, const char* name);
typedef void         (GL_APIENTRY *pfnglGetVertexAttribfv) (GLuint index, GLenum pname, GLfloat* params);
typedef void         (GL_APIENTRY *pfnglGetVertexAttribiv) (GLuint index, GLenum pname, GLint* params);
typedef void         (GL_APIENTRY *pfnglGetVertexAttribPointerv) (GLuint index, GLenum pname, void** pointer);
typedef void         (GL_APIENTRY *pfnglHint) (GLenum target, GLenum mode);
typedef GLboolean    (GL_APIENTRY *pfnglIsBuffer) (GLuint buffer);
typedef GLboolean    (GL_APIENTRY *pfnglIsEnabled) (GLenum cap);
typedef GLboolean    (GL_APIENTRY *pfnglIsFramebuffer) (GLuint framebuffer);
typedef GLboolean    (GL_APIENTRY *pfnglIsProgram) (GLuint program);
typedef GLboolean    (GL_APIENTRY *pfnglIsRenderbuffer) (GLuint renderbuffer);
typedef GLboolean    (GL_APIENTRY *pfnglIsShader) (GLuint shader);
typedef GLboolean    (GL_APIENTRY *pfnglIsTexture) (GLuint texture);
typedef void         (GL_APIENTRY *pfnglLineWidth) (GLfloat width);
typedef void         (GL_APIENTRY *pfnglLinkProgram) (GLuint program);
typedef void         (GL_APIENTRY *pfnglPixelStorei) (GLenum pname, GLint param);
typedef void         (GL_APIENTRY *pfnglPolygonOffset) (GLfloat factor, GLfloat units);
typedef void *		 (GL_APIENTRY *pfnglMapBufferOES)(GLenum target, GLenum access);
typedef void *		  (GL_APIENTRY *pfnglMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean	 (GL_APIENTRY *pfnglUnmapBufferOES)(GLenum target);
typedef GLboolean(GL_APIENTRY *pfnglUnmapBuffer)(GLenum target);
typedef void         (GL_APIENTRY *pfnglReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
typedef void         (GL_APIENTRY *pfnglReleaseShaderCompiler) (void);
typedef void         (GL_APIENTRY *pfnglRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void         (GL_APIENTRY *pfnglSampleCoverage) (GLclampf value, GLboolean invert);
typedef void         (GL_APIENTRY *pfnglScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void         (GL_APIENTRY *pfnglShaderBinary) (GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
typedef void         (GL_APIENTRY *pfnglShaderSource) (GLuint shader, GLsizei count, const char** string, const GLint* length);
typedef void         (GL_APIENTRY *pfnglStencilFunc) (GLenum func, GLint ref, GLuint mask);
typedef void         (GL_APIENTRY *pfnglStencilFuncSeparate) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void         (GL_APIENTRY *pfnglStencilMask) (GLuint mask);
typedef void         (GL_APIENTRY *pfnglStencilMaskSeparate) (GLenum face, GLuint mask);
typedef void         (GL_APIENTRY *pfnglStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void         (GL_APIENTRY *pfnglStencilOpSeparate) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
typedef void         (GL_APIENTRY *pfnglTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void         (GL_APIENTRY *pfnglTexParameterf) (GLenum target, GLenum pname, GLfloat param);
typedef void         (GL_APIENTRY *pfnglTexParameterfv) (GLenum target, GLenum pname, const GLfloat* params);
typedef void         (GL_APIENTRY *pfnglTexParameteri) (GLenum target, GLenum pname, GLint param);
typedef void         (GL_APIENTRY *pfnglTexParameteriv) (GLenum target, GLenum pname, const GLint* params);
typedef void         (GL_APIENTRY *pfnglTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void         (GL_APIENTRY *pfnglUniform1f) (GLint location, GLfloat x);
typedef void         (GL_APIENTRY *pfnglUniform1fv) (GLint location, GLsizei count, const GLfloat* v);
typedef void         (GL_APIENTRY *pfnglUniform1i) (GLint location, GLint x);
typedef void         (GL_APIENTRY *pfnglUniform1iv) (GLint location, GLsizei count, const GLint* v);
typedef void         (GL_APIENTRY *pfnglUniform2f) (GLint location, GLfloat x, GLfloat y);
typedef void         (GL_APIENTRY *pfnglUniform2fv) (GLint location, GLsizei count, const GLfloat* v);
typedef void         (GL_APIENTRY *pfnglUniform2i) (GLint location, GLint x, GLint y);
typedef void         (GL_APIENTRY *pfnglUniform2iv) (GLint location, GLsizei count, const GLint* v);
typedef void         (GL_APIENTRY *pfnglUniform3f) (GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void         (GL_APIENTRY *pfnglUniform3fv) (GLint location, GLsizei count, const GLfloat* v);
typedef void         (GL_APIENTRY *pfnglUniform3i) (GLint location, GLint x, GLint y, GLint z);
typedef void         (GL_APIENTRY *pfnglUniform3iv) (GLint location, GLsizei count, const GLint* v);
typedef void         (GL_APIENTRY *pfnglUniform4f) (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void         (GL_APIENTRY *pfnglUniform4fv) (GLint location, GLsizei count, const GLfloat* v);
typedef void         (GL_APIENTRY *pfnglUniform4i) (GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void         (GL_APIENTRY *pfnglUniform4iv) (GLint location, GLsizei count, const GLint* v);
typedef void         (GL_APIENTRY *pfnglUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void         (GL_APIENTRY *pfnglUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void         (GL_APIENTRY *pfnglUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void         (GL_APIENTRY *pfnglUseProgram) (GLuint program);
typedef void         (GL_APIENTRY *pfnglValidateProgram) (GLuint program);
typedef void         (GL_APIENTRY *pfnglVertexAttrib1f) (GLuint indx, GLfloat x);
typedef void         (GL_APIENTRY *pfnglVertexAttrib1fv) (GLuint indx, const GLfloat* values);
typedef void         (GL_APIENTRY *pfnglVertexAttrib2f) (GLuint indx, GLfloat x, GLfloat y);
typedef void         (GL_APIENTRY *pfnglVertexAttrib2fv) (GLuint indx, const GLfloat* values);
typedef void         (GL_APIENTRY *pfnglVertexAttrib3f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
typedef void         (GL_APIENTRY *pfnglVertexAttrib3fv) (GLuint indx, const GLfloat* values);
typedef void         (GL_APIENTRY *pfnglVertexAttrib4f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void         (GL_APIENTRY *pfnglVertexAttrib4fv) (GLuint indx, const GLfloat* values);
typedef void         (GL_APIENTRY *pfnglVertexAttribPointer) (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
typedef void         (GL_APIENTRY *pfnglViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void         (GL_APIENTRY *pfnglBufferData) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void         (GL_APIENTRY *pfnglBindBuffer) (GLenum target, GLuint buffer);
typedef void         (GL_APIENTRY *pfnglBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef const GLubyte(GL_APIENTRY  *pfnrglGetString) (GLenum name);

#define DECALAREGLVAR(GlClass) GlClass g_old_##GlClass;
extern DECALAREGLVAR(pfnglActiveTexture) 
extern DECALAREGLVAR(pfnglAttachShader) 
extern DECALAREGLVAR(pfnglBindAttribLocation) 
extern DECALAREGLVAR(pfnglBindFramebuffer) 
extern DECALAREGLVAR(pfnglBindRenderbuffer) 
extern DECALAREGLVAR(pfnglBindTexture) 
extern DECALAREGLVAR(pfnglBlendColor)
extern DECALAREGLVAR(pfnglBlendEquation)
extern DECALAREGLVAR(pfnglBlendEquationSeparate)
extern DECALAREGLVAR(pfnglBlendFunc) 
extern DECALAREGLVAR(pfnglBlendFuncSeparate) 
extern DECALAREGLVAR(pfnglCheckFramebufferStatus) 
extern DECALAREGLVAR(pfnglClear) 
extern DECALAREGLVAR(pfnglClearColor) 
extern DECALAREGLVAR(pfnglClearDepthf) 
extern DECALAREGLVAR(pfnglClearStencil) 
extern DECALAREGLVAR(pfnglColorMask) 
extern DECALAREGLVAR(pfnglCompileShader) 
extern DECALAREGLVAR(pfnglCompressedTexImage2D) 
extern DECALAREGLVAR(pfnglCompressedTexSubImage2D) 
extern DECALAREGLVAR(pfnglCopyTexImage2D) 
extern DECALAREGLVAR(pfnglCopyTexSubImage2D) 
extern DECALAREGLVAR(pfnglCreateProgram);
extern DECALAREGLVAR(pfnglCreateShader) 
extern DECALAREGLVAR(pfnglCullFace) 
extern DECALAREGLVAR(pfnglDeleteBuffers) 
extern DECALAREGLVAR(pfnglDeleteFramebuffers) 
extern DECALAREGLVAR(pfnglDeleteTextures) 
extern DECALAREGLVAR(pfnglDeleteProgram) 
extern DECALAREGLVAR(pfnglDeleteRenderbuffers) 
extern DECALAREGLVAR(pfnglDeleteShader) 
extern DECALAREGLVAR(pfnglDetachShader) 
extern DECALAREGLVAR(pfnglDepthFunc) 
extern DECALAREGLVAR(pfnglDepthMask) 
extern DECALAREGLVAR(pfnglDepthRangef) 
extern DECALAREGLVAR(pfnglDisable) 
extern DECALAREGLVAR(pfnglDisableVertexAttribArray) 
extern DECALAREGLVAR(pfnglDrawArrays) 
extern DECALAREGLVAR(pfnglDrawElements) 
extern DECALAREGLVAR(pfnglDrawElementsInstanced)
extern DECALAREGLVAR(pfnglEnable) 
extern DECALAREGLVAR(pfnglEnableVertexAttribArray) 
extern DECALAREGLVAR(pfnglFinish)
extern DECALAREGLVAR(pfnglFlush)
extern DECALAREGLVAR(pfnglFramebufferRenderbuffer) 
extern DECALAREGLVAR(pfnglFramebufferTexture2D) 
extern DECALAREGLVAR(pfnglFrontFace) 
extern DECALAREGLVAR(pfnglGenBuffers) 
extern DECALAREGLVAR(pfnglGenerateMipmap) 
extern DECALAREGLVAR(pfnglGenFramebuffers) 
extern DECALAREGLVAR(pfnglGenRenderbuffers) 
extern DECALAREGLVAR(pfnglGenTextures) 
extern DECALAREGLVAR(pfnglGetActiveAttrib) 
extern DECALAREGLVAR(pfnglGetActiveUniform) 
extern DECALAREGLVAR(pfnglGetAttachedShaders) 
extern DECALAREGLVAR(pfnglGetAttribLocation) 
extern DECALAREGLVAR(pfnglGetBooleanv) 
extern DECALAREGLVAR(pfnglGetBufferParameteriv) 
extern DECALAREGLVAR(pfnglGetError)
extern DECALAREGLVAR(pfnglGetFloatv) 
extern DECALAREGLVAR(pfnglGetFramebufferAttachmentParameteriv) 
extern DECALAREGLVAR(pfnglGetIntegerv) 
extern DECALAREGLVAR(pfnglGetProgramiv) 
extern DECALAREGLVAR(pfnglGetProgramInfoLog) 
extern DECALAREGLVAR(pfnglGetRenderbufferParameteriv) 
extern DECALAREGLVAR(pfnglGetShaderiv) 
extern DECALAREGLVAR(pfnglGetShaderInfoLog) 
extern DECALAREGLVAR(pfnglGetShaderPrecisionFormat) 
extern DECALAREGLVAR(pfnglGetShaderSource) 
extern DECALAREGLVAR(pfnglGetTexParameterfv) 
extern DECALAREGLVAR(pfnglGetTexParameteriv) 
extern DECALAREGLVAR(pfnglGetUniformfv) 
extern DECALAREGLVAR(pfnglGetUniformiv) 
extern DECALAREGLVAR(pfnglGetUniformLocation) 
extern DECALAREGLVAR(pfnglGetVertexAttribfv) 
extern DECALAREGLVAR(pfnglGetVertexAttribiv) 
extern DECALAREGLVAR(pfnglGetVertexAttribPointerv) 
extern DECALAREGLVAR(pfnglHint) 
extern DECALAREGLVAR(pfnglIsBuffer) 
extern DECALAREGLVAR(pfnglIsEnabled) 
extern DECALAREGLVAR(pfnglIsFramebuffer) 
extern DECALAREGLVAR(pfnglIsProgram) 
extern DECALAREGLVAR(pfnglIsRenderbuffer) 
extern DECALAREGLVAR(pfnglIsShader) 
extern DECALAREGLVAR(pfnglIsTexture) 
extern DECALAREGLVAR(pfnglLineWidth) 
extern DECALAREGLVAR(pfnglLinkProgram) 
extern DECALAREGLVAR(pfnglPixelStorei) 
extern DECALAREGLVAR(pfnglPolygonOffset) 
extern DECALAREGLVAR(pfnglReadPixels) 
extern DECALAREGLVAR(pfnglMapBufferOES)
extern DECALAREGLVAR(pfnglMapBufferRange)
extern DECALAREGLVAR(pfnglUnmapBufferOES)
extern DECALAREGLVAR(pfnglUnmapBuffer)
extern DECALAREGLVAR(pfnglReleaseShaderCompiler)
extern DECALAREGLVAR(pfnglRenderbufferStorage) 
extern DECALAREGLVAR(pfnglSampleCoverage) 
extern DECALAREGLVAR(pfnglScissor) 
extern DECALAREGLVAR(pfnglShaderBinary) 
extern DECALAREGLVAR(pfnglShaderSource) 
extern DECALAREGLVAR(pfnglStencilFunc) 
extern DECALAREGLVAR(pfnglStencilFuncSeparate) 
extern DECALAREGLVAR(pfnglStencilMask) 
extern DECALAREGLVAR(pfnglStencilMaskSeparate) 
extern DECALAREGLVAR(pfnglStencilOp) 
extern DECALAREGLVAR(pfnglStencilOpSeparate) 
extern DECALAREGLVAR(pfnglTexImage2D) 
extern DECALAREGLVAR(pfnglTexParameterf) 
extern DECALAREGLVAR(pfnglTexParameterfv) 
extern DECALAREGLVAR(pfnglTexParameteri) 
extern DECALAREGLVAR(pfnglTexParameteriv) 
extern DECALAREGLVAR(pfnglTexSubImage2D) 
extern DECALAREGLVAR(pfnglUniform1f) 
extern DECALAREGLVAR(pfnglUniform1fv) 
extern DECALAREGLVAR(pfnglUniform1i) 
extern DECALAREGLVAR(pfnglUniform1iv) 
extern DECALAREGLVAR(pfnglUniform2f) 
extern DECALAREGLVAR(pfnglUniform2fv) 
extern DECALAREGLVAR(pfnglUniform2i) 
extern DECALAREGLVAR(pfnglUniform2iv) 
extern DECALAREGLVAR(pfnglUniform3f) 
extern DECALAREGLVAR(pfnglUniform3fv) 
extern DECALAREGLVAR(pfnglUniform3i) 
extern DECALAREGLVAR(pfnglUniform3iv) 
extern DECALAREGLVAR(pfnglUniform4f) 
extern DECALAREGLVAR(pfnglUniform4fv) 
extern DECALAREGLVAR(pfnglUniform4i) 
extern DECALAREGLVAR(pfnglUniform4iv) 
extern DECALAREGLVAR(pfnglUniformMatrix2fv) 
extern DECALAREGLVAR(pfnglUniformMatrix3fv) 
extern DECALAREGLVAR(pfnglUniformMatrix4fv) 
extern DECALAREGLVAR(pfnglUseProgram) 
extern DECALAREGLVAR(pfnglValidateProgram) 
extern DECALAREGLVAR(pfnglVertexAttrib1f) 
extern DECALAREGLVAR(pfnglVertexAttrib1fv) 
extern DECALAREGLVAR(pfnglVertexAttrib2f) 
extern DECALAREGLVAR(pfnglVertexAttrib2fv) 
extern DECALAREGLVAR(pfnglVertexAttrib3f) 
extern DECALAREGLVAR(pfnglVertexAttrib3fv) 
extern DECALAREGLVAR(pfnglVertexAttrib4f) 
extern DECALAREGLVAR(pfnglVertexAttrib4fv) 
extern DECALAREGLVAR(pfnglVertexAttribPointer) 
extern DECALAREGLVAR(pfnglViewport) 
extern DECALAREGLVAR(pfnglBufferData) 
extern DECALAREGLVAR(pfnglBindBuffer) 
extern DECALAREGLVAR(pfnglBufferSubData) 
extern DECALAREGLVAR(pfnrglGetString)

EGLContext EGLAPIENTRY DetoureglGetCurrentContext(void);
typedef EGLContext (EGLAPIENTRY *pfneglGetCurrentContext)();
extern DECALAREGLVAR(pfneglGetCurrentContext)

EGLSurface EGLAPIENTRY DetoureglGetCurrentSurface(EGLint readdraw);
typedef EGLSurface (EGLAPIENTRY *pfneglGetCurrentSurface)(EGLint readdraw);
extern DECALAREGLVAR(pfneglGetCurrentSurface)

EGLBoolean EGLAPIENTRY DetoureglSwapBuffers(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean (EGLAPIENTRY *pfneglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
extern DECALAREGLVAR(pfneglSwapBuffers)

EGLContext EGLAPIENTRY DetoureglCreateContext(EGLDisplay dpy, EGLConfig config,
EGLContext share_context,
const EGLint *attrib_list);
typedef EGLContext (EGLAPIENTRY *pfneglCreateContext)(EGLDisplay dpy, EGLConfig config,
													  EGLContext share_context,
													  const EGLint *attrib_list);
extern DECALAREGLVAR(pfneglCreateContext)

template <typename T>
struct Vector3d
{
	T m[3];
	Vector3d()
	{
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
	}

	Vector3d(T x, T y, T z)
	{
		m[0] = x;
		m[1] = y;
		m[2] = z;
	}

	T X()
	{
		return m[0];
	}

	T Y()
	{
		return m[1];
	}

	T Z()
	{
		return m[2];
	}
};

template <typename T>
struct Vector2d
{
	T m[2];
	Vector2d()
	{
		m[0] = 0;
		m[1] = 0;
	}
	Vector2d(T x, T y)
	{
		m[0] = x;
		m[1] = y;
	}

	T X()
	{
		return m[0];
	}

	T Y()
	{
		return m[1];
	}
};
#pragma pack(push,1)
struct VertexAttribParam
{
	GLuint		_indx;
	GLint		_size;
	GLenum		_type;
	GLboolean	_normalized;
	GLsizei		_stride;
	const void* offset;
	VertexAttribParam()
	{
		_indx = 0;
		_size = 0;
		_type = 0;
		_normalized = GL_FALSE;
		_stride = 0;
		offset    = 0;
	}
};
struct IndxAttrib
{
	GLuint _mode;
	GLint  _count;
	GLuint _type;
	GLuint _offset;
};
struct TexAttrib
{
	GLenum		_target;
	GLint		_level;
	GLenum		_internalFormat;
	GLsizei		_width;
	GLsizei		_height;
	GLint		_border;
	GLenum		_format;
	GLenum		_type;
	TexAttrib()
	{
		_target		= 0;
		_level		= 0;
		_internalFormat = 0;
		_width		= 0;
		_height		= 0;
		_border		= 0;
		_format		= 0;
		_type		= 0;
	}
};
#pragma pack(pop)



typedef	signed char			int8;       //!< Signed 8 bit integer
typedef	signed short		int16;      //!< Signed 16 bit integer
typedef	signed int			int32;      //!< Signed 32 bit integer
typedef	signed long long    int64;      //!< Signed 64 bit integer
typedef unsigned char		uint8;      //!< Unsigned 8 bit integer
typedef unsigned short		uint16;     //!< Unsigned 16 bit integer
typedef unsigned int		uint32;     //!< Unsigned 32 bit integer
typedef	unsigned long long	uint64;     //!< Unsigned 64 bit integer
typedef unsigned char		byte;
#endif

