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
//<ID> 329
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "maybe", maybe);
    int ints[] = {1, 2, 3};
    cJSON *int_arr = cJSON_CreateIntArray(ints, 3);
    cJSON_AddItemToObject(root, "ints", int_arr);

    // step 2: Configure
    double set_res = cJSON_SetNumberHelper(num, 456.78);
    cJSON *computed = cJSON_CreateNumber(set_res + 1.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate & Validate
    char *printed_before = cJSON_Print(root);
    cJSON_DeleteItemFromObjectCaseSensitive(child, "maybe");
    char *printed_after = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}