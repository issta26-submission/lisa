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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"alpha\":2.5,\"beta\":[1,2,3]}";
    size_t json_len = strlen(json_text);
    const char *parse_end = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *alpha = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "alpha", alpha);
    cJSON *beta = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "beta", beta);
    cJSON_AddItemToArray(beta, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(beta, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(beta, cJSON_CreateNumber(3));
    cJSON *newnum = cJSON_CreateNumber(0.0);
    cJSON *raw = cJSON_CreateRaw("{\"injected\":42}");
    char *printed = NULL;
    int arr_size = 0;
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);

    // step 2: Setup / Configure
    cJSON_SetNumberHelper(newnum, 99.99);
    cJSON_AddItemToObject(root, "injected_number", newnum);
    cJSON_AddItemToObject(root, "raw_injected", raw);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(beta);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    scratch[0] = (char)('0' + (arr_size % 10));
    const char *pe2 = NULL;
    cJSON *reparsed = cJSON_ParseWithLengthOpts(printed, strlen(printed), &pe2, 1);
    double changed_alpha = cJSON_SetNumberHelper(cJSON_GetObjectItem(root, "alpha"), 7.5);
    scratch[1] = (char)('A' + (((int)changed_alpha) % 26));
    (void)reparsed;
    (void)parse_end;
    (void)json_len;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}