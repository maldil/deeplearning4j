/* ******************************************************************************
 *
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 *  See the NOTICE file distributed with this work for additional
 *  information regarding copyright ownership.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// @author raver119@gmail.com
//
#include <array/NDArray.h>
#include <array/NDArrayList.h>

#include "testlayers.h"

using namespace sd;

class NDArrayListTests : public testing::Test {
 public:
};

TEST_F(NDArrayListTests, BasicTests_1) {
  NDArrayList list(false);

  auto x = NDArrayFactory::create<float>('c', {1, 10});
  auto y = NDArrayFactory::create<float>('c', {1, 10});

  ASSERT_EQ(sd::Status::OK, list.write(1, new NDArray(x.dup())));
}

TEST_F(NDArrayListTests, BasicTests_2) {
  NDArrayList list(false);

  auto x = NDArrayFactory::create<float>('c', {1, 10});
  auto y = NDArrayFactory::create<float>('c', {1, 7});

  ASSERT_EQ(sd::Status::OK, list.write(1, new NDArray(x.dup())));

  ASSERT_EQ(sd::Status::BAD_INPUT, list.write(0, &y));
}

TEST_F(NDArrayListTests, Test_Stack_UnStack_1) {
  auto input = NDArrayFactory::create<float>('c', {10, 10});
  input.linspace(1);

  NDArrayList list(false);

  list.unstack(&input, 0);

  ASSERT_EQ(10, list.elements());

  auto array = list.stack();

  ASSERT_TRUE(input.isSameShape(array));

  ASSERT_TRUE(input.equalsTo(array));

  delete array;
}
