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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *json_text = (char *)cJSON_malloc(64);
    memset(json_text, 0, 64);
    memcpy(json_text, "[ 10.5 , 20 , 30 ]", 19);
    cJSON_Minify(json_text);
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "data", parsed);

    // step 3: Operate / Validate
    int size = cJSON_GetArraySize(parsed);
    cJSON *first_item = cJSON_GetArrayItem(parsed, 0);
    cJSON *last_item = cJSON_GetArrayItem(parsed, size - 1);
    double first_val = cJSON_GetNumberValue(first_item);
    double last_val = cJSON_GetNumberValue(last_item);
    cJSON *first_num = cJSON_CreateNumber(first_val);
    cJSON *last_num = cJSON_CreateNumber(last_val);
    cJSON_AddItemToObject(root, "first_value", first_num);
    cJSON_AddItemToObject(root, "last_value", last_num);

    // step 4: Cleanup
    cJSON_free(json_text);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}