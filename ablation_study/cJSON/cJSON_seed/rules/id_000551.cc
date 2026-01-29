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
//<ID> 551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *msg = "ok";
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_ref = cJSON_CreateStringReference(msg);
    cJSON *base_num = NULL;
    cJSON *combined_num = NULL;
    char *retrieved = NULL;
    char first_char = '\0';
    double combined_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "message", str_ref);
    base_num = cJSON_AddNumberToObject(root, "base", 7.0);

    // step 3: Operate / Validate
    retrieved = cJSON_GetStringValue(str_ref);
    first_char = retrieved[0];
    combined_value = (double)first_char + 3.0;
    combined_num = cJSON_AddNumberToObject(root, "combined", combined_value);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}