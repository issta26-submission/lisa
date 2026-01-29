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
//<ID> 629
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *n1 = cJSON_CreateNumber(4.0);
    cJSON *n2 = cJSON_CreateNumber(6.0);
    cJSON_AddItemToObject(data, "n1", n1);
    cJSON_AddItemToObject(data, "n2", n2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    double v1 = cJSON_GetNumberValue(n1);
    double v2 = cJSON_GetNumberValue(n2);
    double sum = v1 + v2;
    double avg = sum / 2.0;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "avg", avg);
    cJSON_AddNumberToObject(data, "product", v1 * v2);

    // step 3: Operate and Validate
    cJSON *retrieved_avg = cJSON_GetObjectItem(root, "avg");
    double avg_check = cJSON_GetNumberValue(retrieved_avg);
    cJSON_AddNumberToObject(root, "avg_check", avg_check);
    cJSON_bool check_flag_bool = cJSON_IsBool(cJSON_GetObjectItem(root, "flag"));
    cJSON_AddNumberToObject(root, "flag_is_bool", (double)check_flag_bool);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}