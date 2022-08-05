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
