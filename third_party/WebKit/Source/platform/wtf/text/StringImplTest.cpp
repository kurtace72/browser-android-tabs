/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/wtf/text/StringImpl.h"

#include "platform/wtf/text/WTFString.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace WTF {

TEST(StringImplTest, Create8Bit) {
  RefPtr<StringImpl> test_string_impl = StringImpl::Create("1224");
  EXPECT_TRUE(test_string_impl->Is8Bit());
}

TEST(StringImplTest, Latin1CaseFoldTable) {
  LChar symbol = 0xff;
  while (symbol--) {
    EXPECT_EQ(Unicode::FoldCase(symbol),
              StringImpl::kLatin1CaseFoldTable[symbol]);
  }
}

TEST(StringImplTest, LowerASCII) {
  RefPtr<StringImpl> test_string_impl = StringImpl::Create("link");
  EXPECT_TRUE(test_string_impl->Is8Bit());
  EXPECT_TRUE(StringImpl::Create("a\xE1")->Is8Bit());

  EXPECT_TRUE(Equal(test_string_impl.Get(),
                    StringImpl::Create("link")->LowerASCII().Get()));
  EXPECT_TRUE(Equal(test_string_impl.Get(),
                    StringImpl::Create("LINK")->LowerASCII().Get()));
  EXPECT_TRUE(Equal(test_string_impl.Get(),
                    StringImpl::Create("lInk")->LowerASCII().Get()));

  EXPECT_TRUE(Equal(StringImpl::Create("LINK")->LowerUnicode().Get(),
                    StringImpl::Create("LINK")->LowerASCII().Get()));
  EXPECT_TRUE(Equal(StringImpl::Create("lInk")->LowerUnicode().Get(),
                    StringImpl::Create("lInk")->LowerASCII().Get()));

  EXPECT_TRUE(Equal(StringImpl::Create("a\xE1").Get(),
                    StringImpl::Create("A\xE1")->LowerASCII().Get()));
  EXPECT_TRUE(Equal(StringImpl::Create("a\xC1").Get(),
                    StringImpl::Create("A\xC1")->LowerASCII().Get()));

  EXPECT_FALSE(Equal(StringImpl::Create("a\xE1").Get(),
                     StringImpl::Create("a\xC1")->LowerASCII().Get()));
  EXPECT_FALSE(Equal(StringImpl::Create("A\xE1").Get(),
                     StringImpl::Create("A\xC1")->LowerASCII().Get()));

  static const UChar kTest[5] = {0x006c, 0x0069, 0x006e, 0x006b, 0};  // link
  static const UChar kTestCapitalized[5] = {0x004c, 0x0049, 0x004e, 0x004b,
                                            0};  // LINK

  RefPtr<StringImpl> test_string_impl16 = StringImpl::Create(kTest, 4);
  EXPECT_FALSE(test_string_impl16->Is8Bit());

  EXPECT_TRUE(Equal(test_string_impl16.Get(),
                    StringImpl::Create(kTest, 4)->LowerASCII().Get()));
  EXPECT_TRUE(
      Equal(test_string_impl16.Get(),
            StringImpl::Create(kTestCapitalized, 4)->LowerASCII().Get()));

  static const UChar kTestWithNonASCII[3] = {0x0061, 0x00e1, 0};  // a\xE1
  static const UChar kTestWithNonASCIIComparison[3] = {0x0061, 0x00c1,
                                                       0};  // a\xC1
  static const UChar kTestWithNonASCIICapitalized[3] = {0x0041, 0x00e1,
                                                        0};  // A\xE1

  // Make sure we support RefPtr<const StringImpl>.
  RefPtr<const StringImpl> const_ref = test_string_impl->IsolatedCopy();
  DCHECK(const_ref->HasOneRef());
  EXPECT_TRUE(Equal(
      StringImpl::Create(kTestWithNonASCII, 2).Get(),
      StringImpl::Create(kTestWithNonASCIICapitalized, 2)->LowerASCII().Get()));
  EXPECT_FALSE(Equal(
      StringImpl::Create(kTestWithNonASCII, 2).Get(),
      StringImpl::Create(kTestWithNonASCIIComparison, 2)->LowerASCII().Get()));
}

TEST(StringImplTest, UpperASCII) {
  RefPtr<StringImpl> test_string_impl = StringImpl::Create("LINK");
  EXPECT_TRUE(test_string_impl->Is8Bit());
  EXPECT_TRUE(StringImpl::Create("a\xE1")->Is8Bit());

  EXPECT_TRUE(Equal(test_string_impl.Get(),
                    StringImpl::Create("link")->UpperASCII().Get()));
  EXPECT_TRUE(Equal(test_string_impl.Get(),
                    StringImpl::Create("LINK")->UpperASCII().Get()));
  EXPECT_TRUE(Equal(test_string_impl.Get(),
                    StringImpl::Create("lInk")->UpperASCII().Get()));

  EXPECT_TRUE(Equal(StringImpl::Create("LINK")->UpperUnicode().Get(),
                    StringImpl::Create("LINK")->UpperASCII().Get()));
  EXPECT_TRUE(Equal(StringImpl::Create("lInk")->UpperUnicode().Get(),
                    StringImpl::Create("lInk")->UpperASCII().Get()));

  EXPECT_TRUE(Equal(StringImpl::Create("A\xE1").Get(),
                    StringImpl::Create("a\xE1")->UpperASCII().Get()));
  EXPECT_TRUE(Equal(StringImpl::Create("A\xC1").Get(),
                    StringImpl::Create("a\xC1")->UpperASCII().Get()));

  EXPECT_FALSE(Equal(StringImpl::Create("A\xE1").Get(),
                     StringImpl::Create("a\xC1")->UpperASCII().Get()));
  EXPECT_FALSE(Equal(StringImpl::Create("A\xE1").Get(),
                     StringImpl::Create("A\xC1")->UpperASCII().Get()));

  static const UChar kTest[5] = {0x006c, 0x0069, 0x006e, 0x006b, 0};  // link
  static const UChar kTestCapitalized[5] = {0x004c, 0x0049, 0x004e, 0x004b,
                                            0};  // LINK

  RefPtr<StringImpl> test_string_impl16 =
      StringImpl::Create(kTestCapitalized, 4);
  EXPECT_FALSE(test_string_impl16->Is8Bit());

  EXPECT_TRUE(Equal(test_string_impl16.Get(),
                    StringImpl::Create(kTest, 4)->UpperASCII().Get()));
  EXPECT_TRUE(
      Equal(test_string_impl16.Get(),
            StringImpl::Create(kTestCapitalized, 4)->UpperASCII().Get()));

  static const UChar kTestWithNonASCII[3] = {0x0061, 0x00e1, 0};  // a\xE1
  static const UChar kTestWithNonASCIIComparison[3] = {0x0061, 0x00c1,
                                                       0};  // a\xC1
  static const UChar kTestWithNonASCIICapitalized[3] = {0x0041, 0x00e1,
                                                        0};  // A\xE1

  // Make sure we support RefPtr<const StringImpl>.
  RefPtr<const StringImpl> const_ref = test_string_impl->IsolatedCopy();
  DCHECK(const_ref->HasOneRef());
  EXPECT_TRUE(
      Equal(StringImpl::Create(kTestWithNonASCIICapitalized, 2).Get(),
            StringImpl::Create(kTestWithNonASCII, 2)->UpperASCII().Get()));
  EXPECT_FALSE(Equal(
      StringImpl::Create(kTestWithNonASCIICapitalized, 2).Get(),
      StringImpl::Create(kTestWithNonASCIIComparison, 2)->UpperASCII().Get()));
}

}  // namespace WTF
