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
//<ID> 1584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON *wrapper = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(wrapper, str_array);
    cJSON_AddItemToObject(root, "wrapped", wrapper);
    cJSON_AddNumberToObject(root, "initial_count", 3.0);

    // step 2: Operate
    cJSON_DeleteItemFromArray(str_array, 1);
    int remaining = cJSON_GetArraySize(str_array);
    cJSON_AddNumberToObject(root, "remaining", (double)remaining);

    // step 3: Validate
    cJSON_bool hasWrapped = cJSON_HasObjectItem(root, "wrapped");
    cJSON_bool hasInitial = cJSON_HasObjectItem(root, "initial_count");
    double computed = (double)remaining + (double)hasWrapped + (double)hasInitial;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}