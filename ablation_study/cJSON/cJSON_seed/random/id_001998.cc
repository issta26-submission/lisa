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
//<ID> 1998
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[128];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *person1 = NULL;
    cJSON *person2 = NULL;
    cJSON *name1 = NULL;
    cJSON *name2 = NULL;
    cJSON *age1 = NULL;
    cJSON *got_arr = NULL;
    char *printed = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_person1 = 0;
    cJSON_bool added_person2 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_arr = cJSON_AddItemToObject(root, "people", arr);

    // step 3: Core operations (create person objects, add to array)
    person1 = cJSON_CreateObject();
    name1 = cJSON_CreateString("Alice");
    age1 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(person1, "name", name1);
    cJSON_AddItemToObject(person1, "age", age1);
    added_person1 = cJSON_AddItemToArray(arr, person1);

    person2 = cJSON_CreateObject();
    name2 = cJSON_CreateString("Bob");
    cJSON_AddItemToObject(person2, "name", name2);
    added_person2 = cJSON_AddItemToArray(arr, person2);

    // step 4: Inspect and print
    got_arr = cJSON_GetObjectItem(root, "people");
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate (aggregate checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(person1 != NULL);
    validation ^= (int)(person2 != NULL);
    validation ^= (int)(name1 != NULL);
    validation ^= (int)(name2 != NULL);
    validation ^= (int)(age1 != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_person1 != 0);
    validation ^= (int)(added_person2 != 0);
    validation ^= (int)(got_arr == arr);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}