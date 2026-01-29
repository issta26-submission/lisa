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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *detached_raw = (cJSON *)0;
    cJSON *found_num = (cJSON *)0;
    char *json_out = (char *)0;
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "number", num_item);
    raw_item = cJSON_AddRawToObject(root, "raw_payload", "{\"flag\":true,\"count\":3}");
    detached_raw = cJSON_DetachItemFromObject(root, "raw_payload");
    cJSON_AddItemToObject(root, "raw_renamed", detached_raw);

    // step 4: Operate
    found_num = cJSON_GetObjectItem(root, "number");
    extracted_value = cJSON_GetNumberValue(found_num);
    json_out = cJSON_PrintUnformatted(root);

    // step 5: Validate (use values to avoid compiler warnings)
    (void)extracted_value;
    (void)raw_item;
    (void)found_num;
    (void)detached_raw;
    (void)json_out;

    // step 6: Cleanup
    if (json_out) { cJSON_free(json_out); json_out = (char *)0; }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}