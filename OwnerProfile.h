#ifndef OWNERPROFILE_H
#define OWNERPROFILE_H

#include <QDialog>

namespace Ui {
class OwnerProfile;
}

class OwnerProfile : public QDialog
{
    Q_OBJECT
    
public:
    explicit OwnerProfile(QWidget *parent = 0);
    ~OwnerProfile();
    
private:
    Ui::OwnerProfile *ui;
};

#endif // OWNERPROFILE_H
