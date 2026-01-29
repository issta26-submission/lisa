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
//<ID> 2168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    cJSON_bool added = 0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup (create object and a true item, add to object)
    root = cJSON_CreateObject();
    flag = cJSON_CreateTrue();
    added = cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate (serialize and retrieve)
    printed = cJSON_PrintUnformatted(root);
    got_flag = cJSON_GetObjectItem(root, "enabled");
    is_true = cJSON_IsTrue(got_flag);
    buf[0] = (char)(is_true + '0');

    // step 4: Validate (accumulate checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(added != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(is_true != 0);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}