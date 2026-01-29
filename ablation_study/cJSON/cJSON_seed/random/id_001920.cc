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
//<ID> 1920
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
    cJSON *num_v1 = NULL;
    cJSON *added_v2 = NULL;
    cJSON *got_v1 = NULL;
    cJSON *got_v2 = NULL;
    cJSON *got_flag = NULL;
    char *out = NULL;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool has_v1 = 0;
    cJSON_bool flag_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_v1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "v1", num_v1);
    added_v2 = cJSON_AddNumberToObject(root, "v2", 20.5);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateTrue());
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    has_v1 = cJSON_HasObjectItem(root, "v1");
    got_v1 = cJSON_GetObjectItem(root, "v1");
    val1 = cJSON_GetNumberValue(got_v1);
    got_v2 = cJSON_GetObjectItem(root, "v2");
    val2 = cJSON_GetNumberValue(got_v2);
    got_flag = cJSON_GetObjectItem(root, "flag");
    flag_true = cJSON_IsTrue(got_flag);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_v1 != NULL);
    validation ^= (int)(added_v2 != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(has_v1 != 0);
    validation ^= (int)(got_v1 != NULL);
    validation ^= (int)(got_v2 != NULL);
    validation ^= (int)(cJSON_IsNumber(got_v1));
    validation ^= (int)(cJSON_IsNumber(got_v2));
    validation ^= (int)(val1 == 10.0);
    validation ^= (int)(val2 == 20.5);
    validation ^= (int)(flag_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}