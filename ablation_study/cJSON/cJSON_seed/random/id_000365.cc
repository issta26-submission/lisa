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
//<ID> 365
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
    cJSON *detached_item = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_raw = (cJSON *)0;
    double detached_value = 0.0;
    const char *raw_text = "{\"flag\":true}";
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "pi", num_item);
    raw_item = cJSON_AddRawToObject(child, "raw", raw_text);

    // step 4: Operate
    fetched_child = cJSON_GetObjectItem(root, "child");
    detached_item = cJSON_DetachItemFromObject(child, "pi");
    cJSON_AddItemToObject(root, "detached_pi", detached_item);

    // step 5: Validate
    fetched_raw = cJSON_GetObjectItem(child, "raw");
    detached_value = cJSON_GetNumberValue(detached_item);
    printed = cJSON_PrintUnformatted(root);
    (void)fetched_child;
    (void)fetched_raw;
    (void)detached_value;
    (void)raw_item;
    (void)num_item;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}