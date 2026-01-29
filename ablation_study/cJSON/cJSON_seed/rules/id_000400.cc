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
//<ID> 400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(32);
    memset(scratch, 0, 32);
    const char *parse_end = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *snew = cJSON_CreateString("gamma");
    cJSON *parsed = NULL;
    cJSON *arr_parsed = NULL;
    cJSON *arr_item = NULL;
    cJSON *enabled_parsed = NULL;
    cJSON *enabled_added = NULL;
    const char *valstr = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, s1);
    enabled_added = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_ReplaceItemInArray(arr, 1, snew);
    parsed = cJSON_ParseWithLengthOpts((const char *)printed, (size_t)-1, &parse_end, 0);
    arr_parsed = cJSON_GetObjectItem(parsed, "numbers");
    arr_item = cJSON_GetArrayItem(arr_parsed, 1);
    valstr = cJSON_GetStringValue(arr_item);
    enabled_parsed = cJSON_GetObjectItem(parsed, "enabled");
    scratch[0] = valstr[0];
    scratch[1] = (char)('0' + (int)cJSON_IsTrue(enabled_parsed));
    scratch[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    return 66;
    // API sequence test completed successfully
}