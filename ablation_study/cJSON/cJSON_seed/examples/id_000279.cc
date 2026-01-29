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
//<ID> 279
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *flag_true = cJSON_CreateBool((cJSON_bool)1);
    cJSON *flag_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, flag_true);
    cJSON_AddItemToArray(arr, flag_false);
    cJSON_bool add_array_ok = cJSON_AddItemToObjectCS(root, "flags", arr);

    // step 3: Operate & Validate
    cJSON *add_array_ok_flag = cJSON_CreateBool(add_array_ok);
    cJSON_AddItemToObjectCS(root, "add_array_ok", add_array_ok_flag);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)1);
    cJSON *printed_flag = cJSON_CreateBool(printed);
    cJSON_AddItemToObjectCS(root, "printed_ok", printed_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}