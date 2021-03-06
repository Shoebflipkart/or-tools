// Copyright 2011-2014 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <vector>
#include <utility>

#include "base/commandlineflags.h"
#include "constraint_solver/routing.h"
#include "base/join.h"

namespace operations_research {

//  Not very interesting
int64 MyCost(RoutingModel::NodeIndex from, RoutingModel::NodeIndex to) {
  return  (from + to).value();
}
  

void VRP_Partial_Routes(void) {
  //  create multi depots
  std::vector<std::pair<RoutingModel::NodeIndex, RoutingModel::NodeIndex> > depots(4);
  depots[0] = std::make_pair(1,4);
  depots[1] = std::make_pair(3,4);
  depots[2] = std::make_pair(3,7);
  depots[3] = std::make_pair(4,7);
  
  RoutingModel VRP(9, 4, depots);

  VRP.SetCost(NewPermanentCallback(MyCost));

  //  Constructing routes
  std::vector<std::vector<RoutingModel::NodeIndex> > routes(4);
  //  Constructing route 0 : 1 - 0 - 2 - 4
  routes[0].push_back(RoutingModel::NodeIndex(0));
  routes[0].push_back(RoutingModel::NodeIndex(2));
  //  Constructing route 1 : 3 - 5 - 4
  routes[1].push_back(RoutingModel::NodeIndex(5));
  //  Constructing route 2 : 3 - 6 - 7
  routes[2].push_back(RoutingModel::NodeIndex(6));
  //  Constructing route 3 : 4 - 8 - 7
  routes[3].push_back(RoutingModel::NodeIndex(8));

  VRP.CloseModel();

  if (VRP.ApplyLocksToAllVehicles(routes, true)) {
    LG << "Routes successfully locked";
  } else {
    LG << "Routes not successfully locked";
  }
 
  const Assignment* Solution = VRP.Solve();

  for (int p = 0; p < VRP.vehicles(); ++p) {
    LG << "Route: " << p;
    std::string route;
    std::string index_route;
    for (int64 index = VRP.Start(p); !VRP.IsEnd(index); index = Solution->Value(VRP.NextVar(index))) {
      route = StrCat(route, StrCat(VRP.IndexToNode(index).value(), " -> "));
      index_route = StrCat(index_route, StrCat(index, " -> "));
    }
    route = StrCat(route, VRP.IndexToNode(VRP.End(p)).value());
    index_route = StrCat(index_route, VRP.End(p));
    LG << route;
    LG << index_route;
  }
  
}  // void VRP_Partial_Routes(void)

}  // namespace operations_research

int main(int argc, char **argv) {

  google::ParseCommandLineFlags(&argc, &argv, true);
  operations_research::VRP_Partial_Routes();

  return 0;
}  //  main