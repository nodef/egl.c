// 01-basic-rendering.c
// This example demonstrates a basic EGL rendering setup.
#define EGL_IMPLEMENTATION
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <KHR/khrplatform.h>
#include <stdio.h>
#include <stdlib.h>

void checkEglError(const char* msg) {
    EGLint error = eglGetError();
    if (error != EGL_SUCCESS) {
        fprintf(stderr, "%s: EGL error: 0x%x\n", msg, error);
        exit(EXIT_FAILURE);
    }
}

int main() {
    // 1. Initialize EGL
    int version = gladLoaderLoadEGL(EGL_NO_DISPLAY);
    if (version == 0) {
        fprintf(stderr, "Failed to load EGL\n");
        return EXIT_FAILURE;
    }
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    printf("EGL Display: %p\n", (void*)display);
    if (display == EGL_NO_DISPLAY) {
        fprintf(stderr, "eglGetDisplay failed: EGL_NO_DISPLAY\n");
        return EXIT_FAILURE;
    }

    if (!eglInitialize(display, NULL, NULL)) {
        fprintf(stderr, "eglInitialize failed\n");
        return EXIT_FAILURE;
    }

    // 2. Choose an EGL config
    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
    checkEglError("eglChooseConfig");

    // 3. Create a Pbuffer surface
    EGLint pbufferAttribs[] = {
        EGL_WIDTH, 256,
        EGL_HEIGHT, 256,
        EGL_NONE
    };
    EGLSurface surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
    checkEglError("eglCreatePbufferSurface");

    // 4. Bind OpenGL API
    eglBindAPI(EGL_OPENGL_API);
    checkEglError("eglBindAPI");

    // 5. Create an EGL context
    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 0,
        EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    checkEglError("eglCreateContext");

    // 6. Make the context current
    eglMakeCurrent(display, surface, surface, context);
    checkEglError("eglMakeCurrent");

    // Check if eglGetDisplay returned a valid display
    if (display == EGL_NO_DISPLAY) {
        fprintf(stderr, "eglGetDisplay failed: EGL_NO_DISPLAY\n");
        return EXIT_FAILURE;
    }

    // Check if eglInitialize succeeded
    if (!eglInitialize(display, NULL, NULL)) {
        fprintf(stderr, "eglInitialize failed\n");
        return EXIT_FAILURE;
    }

    // Check if eglCreatePbufferSurface returned a valid surface
    if (surface == EGL_NO_SURFACE) {
        fprintf(stderr, "eglCreatePbufferSurface failed: EGL_NO_SURFACE\n");
        return EXIT_FAILURE;
    }

    // Check if eglCreateContext returned a valid context
    if (context == EGL_NO_CONTEXT) {
        fprintf(stderr, "eglCreateContext failed: EGL_NO_CONTEXT\n");
        return EXIT_FAILURE;
    }

    // Check if eglMakeCurrent succeeded
    if (!eglMakeCurrent(display, surface, surface, context)) {
        fprintf(stderr, "eglMakeCurrent failed\n");
        return EXIT_FAILURE;
    }

    // 7. Render something (OpenGL commands go here)
    printf("EGL context and surface successfully created!\n");

    // 8. Cleanup
    eglDestroySurface(display, surface);
    eglDestroyContext(display, context);
    eglTerminate(display);

    gladLoaderUnloadEGL();
    return EXIT_SUCCESS;
}
