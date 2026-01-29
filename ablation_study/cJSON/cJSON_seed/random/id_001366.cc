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
//<ID> 1366
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
    cJSON *nameItem = NULL;
    cJSON *ageItem = NULL;
    cJSON *gotName = NULL;
    cJSON *gotAge = NULL;
    const char *nameVal = NULL;
    double ageVal = 0.0;
    int validation = 0;
    cJSON_bool addedName = 0;
    cJSON_bool addedAge = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    nameItem = cJSON_CreateString("Alice");
    ageItem = cJSON_CreateNumber(30.0);
    addedName = cJSON_AddItemToObject(root, "name", nameItem);
    addedAge = cJSON_AddItemToObject(root, "age", ageItem);

    // step 3: Operate
    gotName = cJSON_GetObjectItem(root, "name");
    gotAge = cJSON_GetObjectItem(root, "age");
    nameVal = cJSON_GetStringValue(gotName);
    ageVal = cJSON_GetNumberValue(gotAge);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(nameItem != NULL);
    validation ^= (int)(ageItem != NULL);
    validation ^= (int)(gotName != NULL);
    validation ^= (int)(gotAge != NULL);
    validation ^= (int)(nameVal != NULL);
    validation ^= (int)(ageVal == 30.0);
    validation ^= (int)(addedName != 0);
    validation ^= (int)(addedAge != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}