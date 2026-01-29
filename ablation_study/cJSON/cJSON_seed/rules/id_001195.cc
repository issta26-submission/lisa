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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *greet = NULL;
    cJSON *flag = NULL;
    const char *greet_val = NULL;
    char *printed = NULL;
    cJSON_bool has_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greet = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greet);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    greet_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    has_flag = cJSON_HasObjectItem(root, "flag");
    printed = cJSON_Print(root);
    (void)greet_val;
    (void)has_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}