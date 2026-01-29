#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double primary_value = 42.5;
    cJSON *topnum_item = cJSON_AddNumberToObject(root, "topnum", primary_value);

    // step 2: Configure
    double extra_value = 3.1415;
    cJSON *extra_item = cJSON_AddNumberToObject(root, "extra", extra_value);

    // step 3: Operate and Validate
    cJSON *found_top = cJSON_GetObjectItem(root, "topnum");
    double extracted = cJSON_GetNumberValue(found_top);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    buf[0] = (char)('A' + ((int)extracted % 26));
    buf[1] = (char)('0' + (int)printed);
    buf[2] = (char)('0' + ((int)extra_value % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}