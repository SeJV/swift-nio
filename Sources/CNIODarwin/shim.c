//===----------------------------------------------------------------------===//
//
// This source file is part of the SwiftNIO open source project
//
// Copyright (c) 2017-2018 Apple Inc. and the SwiftNIO project authors
// Licensed under Apache License v2.0
//
// See LICENSE.txt for license information
// See CONTRIBUTORS.txt for the list of SwiftNIO project authors
//
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#ifdef __APPLE__
#include <c_nio_darwin.h>
#include <err.h>
#include <sysexits.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int CNIODarwin_sendmmsg(int sockfd, CNIODarwin_mmsghdr *msgvec, unsigned int vlen, int flags) {
    // Some quick error checking. If vlen can't fit into int, we bail.
    if ((vlen > INT_MAX) || (msgvec == NULL)) {
        errno = EINVAL;
        return -1;
    }

    for (unsigned int i = 0; i < vlen; i++) {
        ssize_t sendAmount = sendmsg(sockfd, &(msgvec[i].msg_hdr), flags);
        if (sendAmount < 0 && i == 0) {
            // Error on the first send, return the error.
            return -1;
        }

        if (sendAmount < 0) {
            // Error on a later send, return short.
            return i;
        }

        // Send succeeded, save off the bytes written.
        msgvec[i].msg_len = (unsigned int)sendAmount;
    }

    // If we dropped out, we sent everything.
    return vlen;
}

int CNIODarwin_recvmmsg(int sockfd, CNIODarwin_mmsghdr *msgvec, unsigned int vlen, int flags, struct timespec *timeout) {
    errx(EX_SOFTWARE, "recvmmsg shim not implemented on Darwin platforms\n");
}

#endif  // __APPLE__