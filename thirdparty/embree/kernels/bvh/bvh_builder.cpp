// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "bvh_builder.h"

#include <algorithm>

#include "common/math/math.h"
#include "common/math/vec3fa.h"
#include "common/simd/vfloat4_sse2.h"
#include "common/simd/vint4_sse2.h"
#include "common/simd/vuint4_sse2.h"
#include "kernels/builders/bvh_builder_sah.h"
#include "kernels/bvh/bvh.h"
#include "kernels/bvh/bvh_node_aabb.h"
#include "kernels/bvh/bvh_node_aabb_mb.h"
#include "kernels/bvh/bvh_node_qaabb.h"
#include "kernels/common/builder.h"
#include "kernels/common/primref.h"

namespace embree
{
  namespace isa
  {
    template<int N>
    typename BVHN<N>::NodeRef BVHNBuilderVirtual<N>::BVHNBuilderV::build(FastAllocator* allocator, BuildProgressMonitor& progressFunc, PrimRef* prims, const PrimInfo& pinfo, GeneralBVHBuilder::Settings settings)
    {
      auto createLeafFunc = [&] (const PrimRef* prims, const range<size_t>& set, const Allocator& alloc) -> NodeRef {
        return createLeaf(prims,set,alloc);
      };
      
      settings.branchingFactor = N;
      settings.maxDepth = BVH::maxBuildDepthLeaf;
      return BVHBuilderBinnedSAH::build<NodeRef>
        (FastAllocator::Create(allocator),typename BVH::AABBNode::Create2(),typename BVH::AABBNode::Set3(allocator,prims),createLeafFunc,progressFunc,prims,pinfo,settings);
    }


    template<int N>
    typename BVHN<N>::NodeRef BVHNBuilderQuantizedVirtual<N>::BVHNBuilderV::build(FastAllocator* allocator, BuildProgressMonitor& progressFunc, PrimRef* prims, const PrimInfo& pinfo, GeneralBVHBuilder::Settings settings)
    {
      auto createLeafFunc = [&] (const PrimRef* prims, const range<size_t>& set, const Allocator& alloc) -> NodeRef {
        return createLeaf(prims,set,alloc);
      };
            
      settings.branchingFactor = N;
      settings.maxDepth = BVH::maxBuildDepthLeaf;
      return BVHBuilderBinnedSAH::build<NodeRef>
        (FastAllocator::Create(allocator),typename BVH::QuantizedNode::Create2(),typename BVH::QuantizedNode::Set2(),createLeafFunc,progressFunc,prims,pinfo,settings);
    }

    template<int N>
    typename BVHN<N>::NodeRecordMB BVHNBuilderMblurVirtual<N>::BVHNBuilderV::build(FastAllocator* allocator, BuildProgressMonitor& progressFunc, PrimRef* prims, const PrimInfo& pinfo, GeneralBVHBuilder::Settings settings, const BBox1f& timeRange)
    {
      auto createLeafFunc = [&] (const PrimRef* prims, const range<size_t>& set, const Allocator& alloc) -> NodeRecordMB {
        return createLeaf(prims,set,alloc);
      };

      settings.branchingFactor = N;
      settings.maxDepth = BVH::maxBuildDepthLeaf;
      return BVHBuilderBinnedSAH::build<NodeRecordMB>
        (FastAllocator::Create(allocator),typename BVH::AABBNodeMB::Create(),typename BVH::AABBNodeMB::SetTimeRange(timeRange),createLeafFunc,progressFunc,prims,pinfo,settings);
    }

    template struct BVHNBuilderVirtual<4>;
    template struct BVHNBuilderQuantizedVirtual<4>;
    template struct BVHNBuilderMblurVirtual<4>;    

#if defined(__AVX__)
    template struct BVHNBuilderVirtual<8>;
    template struct BVHNBuilderQuantizedVirtual<8>;
    template struct BVHNBuilderMblurVirtual<8>;
#endif
  }
}
