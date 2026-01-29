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
//<ID> 907
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_entry = cJSON_CreateNull();
    cJSON *num_entry = cJSON_CreateNumber(42.0);
    cJSON *str_entry = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "nothing", null_entry);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "flag", (cJSON_bool)1);
    (void)flag_item;
    cJSON_AddItemToObject(root, "answer", num_entry);
    cJSON_AddItemToObject(root, "label", str_entry);

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    cJSON_Minify(pretty);
    cJSON *parsed = cJSON_Parse(pretty);
    cJSON *got_answer = cJSON_GetObjectItem(parsed, "answer");
    double original = cJSON_GetNumberValue(got_answer);
    double doubled = cJSON_SetNumberHelper(got_answer, original * 2.0);
    cJSON *double_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "double_answer", double_item);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}