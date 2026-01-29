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
//<ID> 270
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
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *answer = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, flag_true);
    cJSON_AddItemToArray(arr, flag_false);
    cJSON_AddItemToObjectCS(root, "flags", arr);
    cJSON_AddItemToObjectCS(root, "answer", answer);

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *printed_flag = cJSON_CreateBool(printed_ok);
    cJSON_AddItemToObjectCS(root, "printed", printed_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}