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
//<ID> 863
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_ref = cJSON_CreateStringReference("static string");
    cJSON *temp_num = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "answer", num_item);
    cJSON_AddItemToObjectCS(root, "label", str_ref);
    cJSON_AddItemToObjectCS(root, "temp", temp_num);
    cJSON_SetNumberHelper(temp_num, 3.1415);

    // step 3: Operate & Validate
    double answer_val = cJSON_GetNumberValue(num_item);
    double temp_val = cJSON_GetNumberValue(temp_num);
    cJSON *derived = cJSON_CreateNumber(answer_val + temp_val);
    cJSON_AddItemToObjectCS(root, "derived", derived);
    const char *label_val = cJSON_GetStringValue(str_ref);
    cJSON *summary = cJSON_CreateObject();
    cJSON *label_ref = cJSON_CreateStringReference(label_val);
    cJSON_AddItemToObjectCS(summary, "label_ref", label_ref);
    cJSON_AddItemToObjectCS(root, "summary", summary);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}