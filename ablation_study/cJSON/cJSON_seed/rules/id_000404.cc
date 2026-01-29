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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed1 = NULL;
    char *printed2 = NULL;
    char *outbuf = (char *)cJSON_malloc(128);
    memset(outbuf, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *flag_item = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *replacement = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    flag_item = cJSON_AddBoolToObject(root, "valid", 1);

    // step 3: Operate / Validate
    printed1 = cJSON_Print(root);
    parsed = cJSON_Parse(printed1);
    parsed_arr = cJSON_GetObjectItem(parsed, "numbers");
    replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(parsed_arr, 1, replacement);
    printed2 = cJSON_Print(parsed);
    outbuf[0] = printed2[0];
    outbuf[1] = (char)('0' + (int)cJSON_GetArraySize(parsed_arr));
    outbuf[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(outbuf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}