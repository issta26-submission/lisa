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
//<ID> 635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"x\":10, \"y\":\"z\"}";
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *raw_item = cJSON_AddRawToObject(child, "rawdata", raw_json);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(child, 1);
    cJSON_bool is_obj = cJSON_IsObject(child);
    cJSON_bool cmp = cJSON_Compare(child, dup, 1);
    int meta_flag = (int)(is_obj + cmp);
    cJSON_AddNumberToObject(root, "meta_flag", (double)meta_flag);

    // step 3: Operate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)raw_item;
    cJSON_Delete(dup);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}