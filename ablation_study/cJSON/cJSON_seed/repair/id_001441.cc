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
//<ID> 1441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateFalse());
    cJSON_AddItemToArray(flags, cJSON_CreateNumber(7.0));

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "sequence-test");
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON *third = cJSON_GetArrayItem(flags, 2);
    double third_val = cJSON_GetNumberValue(third);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = len > 0 ? unfmt[0] : ' ';
    scratch[1] = (char)('0' + (first_is_true ? 1 : 0));
    scratch[2] = (char)('0' + (char)(((int)third_val) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}