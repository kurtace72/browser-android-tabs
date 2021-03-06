/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/loader/fetch/ResourceError.h"

#include "platform/loader/fetch/ResourceRequest.h"
#include "platform/weborigin/KURL.h"
#include "public/platform/Platform.h"
#include "public/platform/WebURL.h"
#include "public/platform/WebURLError.h"

namespace blink {

const char kErrorDomainBlinkInternal[] = "BlinkInternal";

ResourceError ResourceError::CancelledError(const String& failing_url) {
  return Platform::Current()->CancelledError(
      KURL(kParsedURLString, failing_url));
}

ResourceError ResourceError::CancelledDueToAccessCheckError(
    const String& failing_url,
    ResourceRequestBlockedReason blocked_reason) {
  ResourceError error = CancelledError(failing_url);
  error.SetIsAccessCheck(true);
  if (blocked_reason == ResourceRequestBlockedReason::kSubresourceFilter)
    error.SetShouldCollapseInitiator(true);
  return error;
}

ResourceError ResourceError::CacheMissError(const String& failing_url) {
  ResourceError error(kErrorDomainBlinkInternal, 0, failing_url, String());
  error.SetIsCacheMiss(true);
  return error;
}

ResourceError ResourceError::Copy() const {
  ResourceError error_copy;
  error_copy.domain_ = domain_.IsolatedCopy();
  error_copy.error_code_ = error_code_;
  error_copy.failing_url_ = failing_url_.IsolatedCopy();
  error_copy.localized_description_ = localized_description_.IsolatedCopy();
  error_copy.is_null_ = is_null_;
  error_copy.is_cancellation_ = is_cancellation_;
  error_copy.is_access_check_ = is_access_check_;
  error_copy.is_timeout_ = is_timeout_;
  error_copy.stale_copy_in_cache_ = stale_copy_in_cache_;
  error_copy.was_ignored_by_handler_ = was_ignored_by_handler_;
  error_copy.is_cache_miss_ = is_cache_miss_;
  return error_copy;
}

bool ResourceError::Compare(const ResourceError& a, const ResourceError& b) {
  if (a.IsNull() && b.IsNull())
    return true;

  if (a.IsNull() || b.IsNull())
    return false;

  if (a.Domain() != b.Domain())
    return false;

  if (a.ErrorCode() != b.ErrorCode())
    return false;

  if (a.FailingURL() != b.FailingURL())
    return false;

  if (a.LocalizedDescription() != b.LocalizedDescription())
    return false;

  if (a.IsCancellation() != b.IsCancellation())
    return false;

  if (a.IsAccessCheck() != b.IsAccessCheck())
    return false;

  if (a.IsTimeout() != b.IsTimeout())
    return false;

  if (a.StaleCopyInCache() != b.StaleCopyInCache())
    return false;

  if (a.WasIgnoredByHandler() != b.WasIgnoredByHandler())
    return false;

  if (a.IsCacheMiss() != b.IsCacheMiss())
    return false;

  return true;
}

}  // namespace blink
