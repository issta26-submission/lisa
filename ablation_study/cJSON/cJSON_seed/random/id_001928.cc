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
//<ID> 1928
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
    cJSON *added_one = NULL;
    cJSON *created_two = NULL;
    cJSON *added_flag = NULL;
    cJSON *got_one = NULL;
    cJSON *got_two = NULL;
    cJSON_bool has_one = 0;
    cJSON_bool has_three = 0;
    cJSON_bool flag_is_true = 0;
    double one_val = 0.0;
    double two_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();

    // step 3: Core operations
    added_one = cJSON_AddNumberToObject(root, "one", 11.5);
    created_two = cJSON_CreateNumber(22.5);
    cJSON_AddItemToObject(root, "two", created_two);
    added_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", added_flag);

    // step 4: Validate
    got_one = cJSON_GetObjectItem(root, "one");
    got_two = cJSON_GetObjectItem(root, "two");
    has_one = cJSON_HasObjectItem(root, "one");
    has_three = cJSON_HasObjectItem(root, "three");
    flag_is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    one_val = cJSON_GetNumberValue(got_one);
    two_val = cJSON_GetNumberValue(got_two);
    validation ^= (int)(root != NULL);
    validation ^= (int)(added_one != NULL);
    validation ^= (int)(created_two != NULL);
    validation ^= (int)(got_one != NULL);
    validation ^= (int)(got_two != NULL);
    validation ^= (int)(has_one != 0);
    validation ^= (int)(has_three == 0);
    validation ^= (int)(flag_is_true != 0);
    validation ^= (int)(one_val == 11.5);
    validation ^= (int)(two_val == 22.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}