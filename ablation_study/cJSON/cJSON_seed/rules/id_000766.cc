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
//<ID> 766
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    const char *json = "{\"num\":3.14}";
    size_t json_len = strlen(json);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *added_num = NULL;
    cJSON *false_item = NULL;
    double extracted = 0.0;
    double copied_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", false_item);
    num_item = cJSON_GetObjectItem(parsed, "num");

    // step 3: Operate / Validate
    extracted = cJSON_GetNumberValue(num_item);
    added_num = cJSON_AddNumberToObject(root, "copied_num", extracted);
    copied_value = cJSON_GetNumberValue(added_num);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}