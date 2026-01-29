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
//<ID> 1921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *created_num = NULL;
    cJSON *added_num = NULL;
    cJSON *got_created = NULL;
    cJSON *got_added = NULL;
    cJSON *got_flag = NULL;
    char *out = NULL;
    double val_created = 0.0;
    double val_added = 0.0;
    cJSON_bool has_created = 0;
    cJSON_bool has_added = 0;
    cJSON_bool flag_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    added_num = cJSON_AddNumberToObject(root, "added", 100.0);
    created_num = cJSON_CreateNumber(7.25);
    cJSON_AddItemToObject(root, "created", created_num);
    cJSON_AddTrueToObject(root, "flag");
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    has_added = cJSON_HasObjectItem(root, "added");
    has_created = cJSON_HasObjectItem(root, "created");
    got_added = cJSON_GetObjectItem(root, "added");
    got_created = cJSON_GetObjectItem(root, "created");
    got_flag = cJSON_GetObjectItem(root, "flag");
    flag_true = cJSON_IsTrue(got_flag);
    val_added = cJSON_GetNumberValue(got_added);
    val_created = cJSON_GetNumberValue(got_created);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_num != NULL);
    validation ^= (int)(created_num != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(has_added != 0);
    validation ^= (int)(has_created != 0);
    validation ^= (int)(got_added != NULL);
    validation ^= (int)(got_created != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(flag_true != 0);
    validation ^= (int)(val_added == 100.0);
    validation ^= (int)(val_created == 7.25);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}