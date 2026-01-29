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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *initial_num = cJSON_CreateNumber(0.0);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "initial_number", initial_num);
    cJSON_AddItemToObject(root, "nullable", nullable);

    // step 2: Configure
    double set1 = cJSON_SetNumberHelper(initial_num, 123.45);
    cJSON *computed = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(computed, set1 * 2.0);
    cJSON_AddItemToObject(meta, "computed", computed);

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "nullable");
    cJSON *readded_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", readded_null);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}