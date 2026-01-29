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
//<ID> 1993
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
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *got_letters = NULL;
    cJSON *got_greeting = NULL;
    char *unformatted = NULL;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_greet = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");

    // step 3: Core operations
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_s2 = cJSON_AddItemToArray(arr, s2);
    added_arr = cJSON_AddItemToObject(root, "letters", arr);
    added_greet = cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_letters = cJSON_GetObjectItem(root, "letters");
    got_greeting = cJSON_GetObjectItem(root, "greeting");
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(s1 != NULL);
    validation ^= (int)(s2 != NULL);
    validation ^= (int)(added_s1 != 0);
    validation ^= (int)(added_s2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_greet != 0);
    validation ^= (int)(got_letters != NULL);
    validation ^= (int)(got_greeting != NULL);
    validation ^= (int)(unformatted != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}