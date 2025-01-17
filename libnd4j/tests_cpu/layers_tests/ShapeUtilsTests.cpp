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
// Created by raver119 on 01.11.2017.
//
#include <array/NDArray.h>
#include <helpers/ShapeUtils.h>

#include "testlayers.h"

using namespace sd;
using namespace sd::graph;

class ShapeUtilsTests : public testing::Test {
 public:
};

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalDimsToExclude_1) {
  std::vector<sd::LongType> zero = {0};
  std::vector<sd::LongType> *res = ShapeUtils::evalDimsToExclude(3,1,zero.data());

  ASSERT_EQ(2, res->size());
  ASSERT_EQ(1, res->at(0));
  ASSERT_EQ(2, res->at(1));

  delete res;
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalDimsToExclude_2) {
  std::vector<sd::LongType> dims = {2, 3};
  std::vector<sd::LongType>* res = ShapeUtils::evalDimsToExclude(4, 2,dims.data());

  ASSERT_EQ(2, res->size());
  ASSERT_EQ(0, res->at(0));
  ASSERT_EQ(1, res->at(1));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, EvalBroadcastShapeInfo_1) {
  sd::LongType xShapeInfo[] = {3, 3, 2, 2, 4, 2, 1, 8192, 1, 99};
  sd::LongType yShapeInfo[] = {2, 1, 2, 2, 1, 8192, 1, 99};
  sd::LongType expShapeInfo[] = {3, 3, 2, 2, 4, 2, 1, 8192, 1, 99};

  NDArray x(xShapeInfo);
  NDArray y(yShapeInfo);

  const sd::LongType *newShapeInfo = nullptr;
  ShapeUtils::evalBroadcastShapeInfo(x, y, false, newShapeInfo, nullptr);

  ASSERT_TRUE(shape::equalsStrict(expShapeInfo, newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, EvalBroadcastShapeInfo_2) {
  sd::LongType xShapeInfo[] = {4, 8, 1, 6, 1, 6, 6, 1, 1, 8192, 1, 99};
  sd::LongType yShapeInfo[] = {3, 7, 1, 5, 5, 5, 1, 8192, 1, 99};
  sd::LongType expShapeInfo[] = {4, 8, 7, 6, 5, 210, 30, 5, 1, 8192, 1, 99};

  NDArray x(xShapeInfo);
  NDArray y(yShapeInfo);

  const sd::LongType *newShapeInfo = nullptr;
  ShapeUtils::evalBroadcastShapeInfo(x, y, false, newShapeInfo, nullptr);

  ASSERT_TRUE(shape::equalsStrict(expShapeInfo, newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, EvalBroadcastShapeInfo_3) {
  sd::LongType xShapeInfo[] = {3, 15, 3, 5, 15, 5, 1, 8192, 1, 99};
  sd::LongType yShapeInfo[] = {3, 15, 1, 5, 5, 5, 1, 8192, 1, 99};
  sd::LongType expShapeInfo[] = {3, 15, 3, 5, 15, 5, 1, 8192, 1, 99};

  NDArray x(xShapeInfo);
  NDArray y(yShapeInfo);

  const sd::LongType *newShapeInfo = nullptr;
  ShapeUtils::evalBroadcastShapeInfo(x, y, false, newShapeInfo, nullptr);

  ASSERT_TRUE(shape::equalsStrict(expShapeInfo, newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, EvalBroadcastShapeInfo_4) {
  sd::LongType xShapeInfo[] = {3, 8, 1, 3, 3, 3, 1, 8192, 1, 99};
  sd::LongType yShapeInfo[] = {2, 4, 3, 3, 1, 8192, 1, 99};
  sd::LongType expShapeInfo[] = {3, 8, 4, 3, 12, 3, 1, 8192, 1, 99};

  NDArray x(xShapeInfo);
  NDArray y(yShapeInfo);

  const sd::LongType *newShapeInfo = nullptr;
  ShapeUtils::evalBroadcastShapeInfo(x, y, false, newShapeInfo, nullptr);
  ASSERT_TRUE(shape::equalsStrict(expShapeInfo, newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalReduceShapeInfo_test1) {
  auto x = NDArrayFactory::create<float>('c', {2, 3, 4, 5});
  auto expected = NDArrayFactory::create<float>('c', {2, 4, 5});
  std::vector<sd::LongType> dimensions = {1};

  auto newShapeInfo = ShapeUtils::evalReduceShapeInfo('c', &dimensions, x.shapeInfo());

  ASSERT_TRUE(shape::shapeEquals(expected.shapeInfo(), newShapeInfo));
}


TEST_F(ShapeUtilsTests, evalReduceShapeInfo_test6) {
  auto x = NDArrayFactory::create<float>('c', {0,1});
  std::vector<sd::LongType> zero = {0};
  auto expected = NDArrayFactory::create<float>('c', zero);
  std::vector<sd::LongType> dimensions = {1};

  auto newShapeInfo = ShapeUtils::evalReduceShapeInfo('c', &dimensions, x.shapeInfo(),false);

  ASSERT_TRUE(shape::shapeEquals(expected.shapeInfo(), newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalReduceShapeInfo_test2) {
  auto x = NDArrayFactory::create<float>('c', {2, 3, 4, 5});
  auto expected = NDArrayFactory::create<float>('c', {2, 1, 4, 5});
  std::vector<sd::LongType> dimensions = {1};

  auto newShapeInfo = ShapeUtils::evalReduceShapeInfo('c', &dimensions, x.shapeInfo(), true);

  ASSERT_TRUE(shape::shapeEquals(expected.shapeInfo(), newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalReduceShapeInfo_test3) {
  auto x = NDArrayFactory::create<float>('c', {2, 3, 4, 5});
  auto expected = NDArrayFactory::create<float>('c', {1, 1, 1, 5});
  std::vector<sd::LongType> dimensions = {0, 1, 2};

  auto newShapeInfo = ShapeUtils::evalReduceShapeInfo('c', &dimensions, x.shapeInfo(), true);

  ASSERT_TRUE(shape::shapeEquals(expected.shapeInfo(), newShapeInfo));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalReduceShapeInfo_test4) {
  auto x = NDArrayFactory::create<float>('c', {2, 3, 4, 5});
  auto expected = NDArrayFactory::create<float>('c', {1, 1, 1, 1});
  std::vector<sd::LongType> dimensions = {0, 1, 2, 3};

  auto newShapeInfo = ShapeUtils::evalReduceShapeInfo('c', &dimensions, x.shapeInfo(), true);

  ASSERT_TRUE(shape::shapeEquals(expected.shapeInfo(), newShapeInfo));
}

TEST_F(ShapeUtilsTests, Test_Strings_1) {
  auto x = NDArrayFactory::create<float>('c', {2, 3, 4, 5});
  std::string exp("[2, 3, 4, 5]");

  auto s = ShapeUtils::shapeAsString(&x);

  ASSERT_EQ(exp, s);
}

TEST_F(ShapeUtilsTests, Test_Backward_Axis_1) {
  auto x = NDArrayFactory::create<float>('c', {2, 4, 3});
  auto y = NDArrayFactory::create<float>('c', {4, 3});
  std::vector<sd::LongType> exp({0});

  auto z = ShapeUtils::evalBroadcastBackwardAxis(y.shapeInfo(), x.shapeInfo());

  ASSERT_EQ(exp, z);
}

TEST_F(ShapeUtilsTests, Test_Backward_Axis_2) {
  auto x = NDArrayFactory::create<float>('c', {2, 4, 4, 3});
  auto y = NDArrayFactory::create<float>('c', {4, 1, 3});
  std::vector<sd::LongType> exp({0, 2});

  auto z = ShapeUtils::evalBroadcastBackwardAxis(y.shapeInfo(), x.shapeInfo());

  ASSERT_EQ(exp, z);
}

TEST_F(ShapeUtilsTests, Test_Backward_Axis_3) {
  auto x = NDArrayFactory::create<float>('c', {2, 4, 4, 3});
  auto y = NDArrayFactory::create<float>('c', {2, 1, 1, 3});
  std::vector<sd::LongType> exp({1, 2});

  auto z = ShapeUtils::evalBroadcastBackwardAxis(y.shapeInfo(), x.shapeInfo());

  ASSERT_EQ(exp, z);
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalPermutFromTo_test1) {
  int a = 1, b = 2, c = 3, d = 4;
  std::vector<sd::LongType> expected = {2, 3, 0, 1};

  std::vector<sd::LongType> result = ShapeUtils::evalPermutFromTo({a, b, c, d}, {c, d, a, b});

  ASSERT_TRUE(std::equal(begin(expected), end(expected), begin(result)));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalPermutFromTo_test2) {
  int a = 1, b = 2, c = 3, d = 4;
  std::vector<sd::LongType> expected = {0, 1, 3, 2};

  std::vector<sd::LongType> result = ShapeUtils::evalPermutFromTo({a, b, c, d}, {a, b, d, c});

  ASSERT_TRUE(std::equal(begin(expected), end(expected), begin(result)));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalPermutFromTo_test3) {
  int a = 2, b = 2, c = 3, d = 2;
  std::vector<sd::LongType> expected = {0, 1, 3, 2};

  std::vector<sd::LongType> result = ShapeUtils::evalPermutFromTo({a, b, c, d}, {a, b, d, c});

  ASSERT_TRUE(std::equal(begin(expected), end(expected), begin(result)));
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalPermutFromTo_test4) {
  int a = 2, b = 3, c = 4, d = 5;

  std::vector<sd::LongType> result = ShapeUtils::evalPermutFromTo({a, b, c, d}, {a, b, c, d});

  ASSERT_TRUE(result.empty());
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalPermutFromTo_test5) {
  int a = 1, b = 2, c = 3, d = 4;
  ASSERT_TRUE(1);
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, evalPermutFromTo_test6) {
  int a = 1, b = 2, c = 3, d = 4;

  // EXPECT_THROW(ShapeUtils::evalPermutFromTo({a,b,c,d}, {a,b,c,d,d}), const char*);
  ASSERT_TRUE(1);
}

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, isPermutNecessary_test1) { ASSERT_TRUE(ShapeUtils::isPermutNecessary({1, 0, 2, 3})); }

//////////////////////////////////////////////////////////////////
TEST_F(ShapeUtilsTests, isPermutNecessary_test2) { ASSERT_TRUE(!ShapeUtils::isPermutNecessary({0, 1, 2, 3})); }
