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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *printed2 = NULL;
    char *scratch = (char *)cJSON_malloc(32);
    memset(scratch, 0, 32);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *newn = cJSON_CreateNumber(99.0);
    cJSON *parsed = NULL;
    cJSON *arr_parsed = NULL;
    const char *parse_end = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    double val_repl = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "ok", 1);
    (void)flag_item;
    printed = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Operate / Validate
    parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed), &parse_end, 1);
    arr_parsed = cJSON_GetObjectItem(parsed, "values");
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr_parsed, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr_parsed, 1));
    cJSON_ReplaceItemInArray(arr_parsed, 1, newn);
    val_repl = cJSON_GetNumberValue(cJSON_GetArrayItem(arr_parsed, 1));
    scratch[0] = (char)('0' + (int)(val0 / 10.0));
    scratch[1] = (char)('0' + (int)(val1 / 10.0));
    scratch[2] = (char)('0' + (int)(val_repl / 10.0));
    scratch[3] = '\0';
    printed2 = cJSON_PrintBuffered(parsed, 64, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}