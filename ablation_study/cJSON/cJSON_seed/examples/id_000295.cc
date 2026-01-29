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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *active = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "name", name);
    cJSON_AddItemToObjectCS(root, "age", age);
    cJSON_AddItemToObjectCS(root, "active", active);

    // step 3: Operate & Validate
    cJSON_bool has_age = cJSON_HasObjectItem(root, "age");
    cJSON *has_age_json = cJSON_CreateBool(has_age);
    cJSON_AddItemToObjectCS(root, "has_age", has_age_json);
    cJSON *age_item = cJSON_GetObjectItem(root, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    cJSON *age_double = cJSON_CreateNumber(age_val * 2.0);
    cJSON_AddItemToObjectCS(root, "age_double", age_double);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_pre = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)1);
    cJSON *printed_flag = cJSON_CreateBool(printed_pre);
    cJSON_AddItemToObjectCS(root, "printed_preallocated", printed_flag);
    char *printed_heap = cJSON_Print(root);
    cJSON_free(printed_heap);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}