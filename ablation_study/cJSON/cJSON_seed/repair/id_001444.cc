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
//<ID> 1444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddTrueToObject(meta, "enabled");
    cJSON_AddItemToArray(values, cJSON_CreateTrue());
    cJSON_AddItemToArray(values, cJSON_CreateNumber(100.0));
    cJSON_AddItemToArray(values, cJSON_CreateString("entry"));

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "cJSON_api_sequence");
    cJSON_AddNumberToObject(root, "expected", 100.0);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double second_value = cJSON_GetNumberValue(second);
    double computed = second_value + (double)first_is_true;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = len > 0 ? unfmt[0] : ' ';
    scratch[1] = (char)('0' + ((int)computed % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}