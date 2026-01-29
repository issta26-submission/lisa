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
//<ID> 1560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *created_num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "created", created_num);
    cJSON_AddNumberToObject(root, "direct", 5.5);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());

    // step 2: Configure
    cJSON *got_created = cJSON_GetObjectItem(root, "created");
    cJSON *got_direct = cJSON_GetObjectItem(root, "direct");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    double v_created = cJSON_GetNumberValue(got_created);
    double v_direct = cJSON_GetNumberValue(got_direct);
    cJSON_bool flag_true = cJSON_IsTrue(got_flag);

    // step 3: Operate & Validate
    double total = v_created + v_direct + (double)flag_true;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}