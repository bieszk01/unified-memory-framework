/*
 *
 * Copyright (C) 2024 Intel Corporation
 *
 * Under the Apache License v2.0 with LLVM Exceptions. See LICENSE.TXT.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 */

#include <hwloc.h>

#include "base_alloc_global.h"
#include "utils_concurrency.h"

static hwloc_topology_t topology = NULL;
static UTIL_ONCE_FLAG topology_initialized = UTIL_ONCE_FLAG_INIT;

void umfDestroyTopology(void) {
    if (topology) {
        hwloc_topology_destroy(topology);
    }
}

static void umfCreateTopology(void) {
    if (hwloc_topology_init(&topology)) {
        fprintf(stderr, "Error: Failed to initialize topology\n");
        topology = NULL;
        return;
    }

    if (hwloc_topology_load(topology)) {
        fprintf(stderr, "Error: Failed to initialize topology\n");
        hwloc_topology_destroy(topology);
        topology = NULL;
    }

#if defined(_WIN32) && !defined(UMF_SHARED_LIBRARY)
    atexit(umfDestroyTopology);
#endif
}

hwloc_topology_t umfGetTopology(void) {
    util_init_once(&topology_initialized, umfCreateTopology);
    return topology;
}
