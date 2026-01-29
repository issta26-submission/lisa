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
//<ID> 2178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *explicit_true = NULL;
    cJSON *explicit_false = NULL;
    cJSON *added_true = NULL;
    cJSON *added_false = NULL;
    char *printed = NULL;
    cJSON_bool is_array = 0;
    cJSON_bool is_true = 0;
    cJSON_bool is_false = 0;
    int validation = 0;

    // step 2: Setup (create object and boolean items)
    root = cJSON_CreateObject();
    explicit_true = cJSON_CreateTrue();
    explicit_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "explicitTrue", explicit_true);
    cJSON_AddItemToObject(root, "explicitFalse", explicit_false);
    added_true = cJSON_AddBoolToObject(root, "flagTrue", 1);
    added_false = cJSON_AddBoolToObject(root, "flagFalse", 0);

    // step 3: Operate (serialize and inspect types)
    printed = cJSON_PrintUnformatted(root);
    is_array = cJSON_IsArray(root);
    is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flagTrue"));
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flagFalse"));

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(explicit_true != NULL);
    validation ^= (int)(explicit_false != NULL);
    validation ^= (int)(added_true != NULL);
    validation ^= (int)(added_false != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(is_array == 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(is_false != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}