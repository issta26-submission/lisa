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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(10.5);
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON *got_num1 = NULL;
    cJSON *got_nested = NULL;
    cJSON *got_num2 = NULL;
    char *printed = NULL;
    double val1 = 0.0;
    double val2 = 0.0;
    double sum = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "num1", num1);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "num2", num2);
    cJSON_AddItemToObject(root, "raw_blob", raw);

    // step 3: Operate / Validate
    got_num1 = cJSON_GetObjectItem(root, "num1");
    val1 = cJSON_GetNumberValue(got_num1);
    got_nested = cJSON_GetObjectItem(root, "nested");
    got_num2 = cJSON_GetObjectItem(got_nested, "num2");
    val2 = cJSON_GetNumberValue(got_num2);
    sum = val1 + val2;
    cJSON_AddNumberToObject(root, "sum", sum);
    equal = cJSON_Compare(got_num1, got_num2, 1);
    cJSON_AddNumberToObject(root, "equal_flag", (double)equal);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}