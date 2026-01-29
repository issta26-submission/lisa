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
//<ID> 1442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddTrueToObject(settings, "flag");
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToArray(values, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(values, cJSON_CreateString("hello"));

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "fuzz-test");
    cJSON_AddNumberToObject(root, "count_expected", 3.0);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = len > 0 ? unfmt[0] : ' ';
    scratch[1] = (char)('0' + (first_is_true ? 1 : 0));
    double second_num = cJSON_GetNumberValue(second);
    scratch[2] = (char)('0' + ((int)second_num % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}