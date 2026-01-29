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
//<ID> 811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *str = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "value_num", num);
    cJSON_AddStringToObject(child, "label", "example_label");
    cJSON *inner = cJSON_AddObjectToObject(child, "inner");
    cJSON_AddStringToObject(inner, "note", "inner_note");

    // step 3: Operate & Validate
    double updated = cJSON_SetNumberHelper(num, 123.456);
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);
    (void)updated;
    (void)str;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}