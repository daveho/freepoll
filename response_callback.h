// Copyright (c) 2022, David Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of FreePoll.
//
// FreePoll is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// FreePoll is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with FreePoll. If not, see <https://www.gnu.org/licenses/>.

#ifndef RESPONSE_CALLBACK_H
#define RESPONSE_CALLBACK_H

#include "datatypes.h"

class ResponseCallback {
private:
  // value semantics are disabled
  ResponseCallback(const ResponseCallback &);
  ResponseCallback &operator=(const ResponseCallback &);

public:
  ResponseCallback();
  virtual ~ResponseCallback();

  // A ResponseCallback's on_response() member function is called from
  // Base::collect_responses() to indicate that a student response has
  // been collected. Note that the call will almost certainly happen
  // in a dedicated thread, so the implementation of on_response()
  // will need to use appropriate synchronization to record the data.
  virtual void on_response(RemoteID remote_id, Option option) = 0;
};


#endif // RESPONSE_CALLBACK_H
