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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    char *json_root = (char *)0;
    char *json_detached = (char *)0;
    const char *raw_json = "{\"flag\":true}";
    double extracted_number = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "num", num_item);
    raw_item = cJSON_AddRawToObject(child, "raw", raw_json);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "child");
    got_num = cJSON_GetObjectItem(got_child, "num");
    extracted_number = cJSON_GetNumberValue(got_num);
    json_root = cJSON_PrintUnformatted(root);
    detached = cJSON_DetachItemFromObject(got_child, "raw");
    json_detached = cJSON_PrintUnformatted(detached);

    // step 5: Validate
    (void)extracted_number;
    (void)json_root;
    (void)json_detached;
    (void)raw_item;
    (void)got_num;

    // step 6: Cleanup
    cJSON_free(json_root);
    cJSON_free(json_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}