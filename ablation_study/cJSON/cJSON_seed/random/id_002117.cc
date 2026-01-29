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
//<ID> 2117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *flag_item = NULL;
    cJSON *got_n1 = NULL;
    cJSON *got_n2 = NULL;
    cJSON *new_num = NULL;
    double val_n1 = 0.0;
    double val_n2 = 0.0;
    cJSON_bool flag_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    n1 = cJSON_AddNumberToObject(root, "n1", 10.0);
    n2 = cJSON_AddNumberToObject(root, "n2", 20.0);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate
    got_n1 = cJSON_GetObjectItemCaseSensitive(root, "n1");
    got_n2 = cJSON_GetObjectItemCaseSensitive(root, "n2");
    val_n1 = cJSON_GetNumberValue(got_n1);
    val_n2 = cJSON_GetNumberValue(got_n2);
    flag_true = cJSON_IsTrue(flag_item);
    new_num = cJSON_CreateNumber(30.0);
    cJSON_ReplaceItemViaPointer(root, got_n1, new_num);
    got_n1 = cJSON_GetObjectItemCaseSensitive(root, "n1");
    val_n1 = cJSON_GetNumberValue(got_n1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(new_num != NULL);
    validation ^= (int)(got_n1 != NULL);
    validation ^= (int)(got_n2 != NULL);
    validation ^= (int)(val_n1 == 30.0);
    validation ^= (int)(val_n2 == 20.0);
    validation ^= (int)(flag_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}