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
//<ID> 1914
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
    cJSON *num_item = NULL;
    cJSON *got_answer = NULL;
    char *set_ret = NULL;
    char *out = NULL;
    double answer_val = 0.0;
    cJSON_bool add_ret = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    set_ret = cJSON_SetValuestring(num_item, "forty-two");

    // step 4: Operate
    add_ret = cJSON_AddItemToObject(root, "answer", num_item);
    out = cJSON_PrintUnformatted(root);

    // step 5: Validate
    got_answer = cJSON_GetObjectItem(root, "answer");
    answer_val = cJSON_GetNumberValue(got_answer);
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(set_ret != NULL);
    validation ^= (int)(add_ret != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_answer != NULL);
    validation ^= (int)(cJSON_IsNumber(got_answer));
    validation ^= (int)(answer_val == 42.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}